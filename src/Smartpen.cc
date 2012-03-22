/* -*- Mode: C++; coding: utf-8; indent-tabs-mode: nil; c-basic-offset: 4; tab-width: 4 -*- */
/* Smartpen.cc
 * This file is part of LibreScribe.
 *
 * LibreScribe is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * LibreScribe is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LibreScribe.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <cstdio>

#include "Smartpen.h"

struct libusb_device_handle *findSmartpen()
{
    //reference: http://www.dreamincode.net/forums/topic/148707-introduction-to-using-libusb-10/
    libusb_device **devs = NULL;//pointer to pointer of device, used to retrieve a list of devices
    libusb_context *ctx = NULL; //a libusb session
    int r; //for return values
    ssize_t cnt; //holding number of devices in list
    
    std::cout << std::endl << "entering findSmartpen()" << std::endl;
    std::cout << "initializing libusb..." << std::endl;
    
    r = libusb_init(&ctx); // Initialize a library session
    libusb_set_debug(ctx, 3); // Set verbosity level to 3, as suggested in the documentation
    
    std::cout << "getting device list..." << std::endl;
    
    cnt = libusb_get_device_list(ctx, &devs); // Get the list of devices
    
    if (cnt >= 0)
    {
        ssize_t i; // For iterating through the list
        libusb_device_descriptor descriptor; // For getting information on the devices
        
        std::cout << "device count: " << cnt << std::endl;
        std::cout << "checking for livescribe pen..." << std::endl;
        
        for (i = 0; i < cnt; i++)
        {
            libusb_get_device_descriptor(devs[i], &descriptor);
            
            if ((descriptor.idVendor == LS_VENDOR_ID))
            {
                static struct libusb_device_handle *devHandle = NULL;
                libusb_open(devs[i], &devHandle);
                libusb_reset_device(devHandle);
                
                std::cout << std::endl << "exiting findSmartpen() "
                          << "returning device" << std::endl;
                
                return devHandle;
            }
        }
        
        std::cout << "no devices match. freeing device list..." << std::endl;
        
        libusb_free_device_list(devs, 1); // Free the list, unref the devices in it
        libusb_exit(ctx);
    }
    else
    {
        std::cerr << "Get device error" << std::endl; // There was an error.
    }
    
    std::cout << std::endl << "exiting findSmartpen() "
              << "returning NULL" << std::endl;
    
    return NULL;
}

static void
obex_requestdone(struct obex_state* state, obex_t* hdl,
                 obex_object_t* obj, int cmd, int resp)
{
    uint8_t header_id;
    obex_headerdata_t hdata;
    uint32_t hlen;

    switch (cmd & ~OBEX_FINAL)
    {
    case OBEX_CMD_CONNECT:
        while (OBEX_ObjectGetNextHeader(hdl, obj, &header_id,
                                        &hdata, &hlen))
        {
            if (header_id == OBEX_HDR_CONNECTION)
            {
                state->got_connid = 1;
                state->connid = hdata.bq4;
//                  printf("Connection ID: %d\n", state->connid);
            }
        }
        
        break;
    case OBEX_CMD_GET:
        while (OBEX_ObjectGetNextHeader(hdl, obj, &header_id,
                                        &hdata, &hlen))
        {
            if (header_id == OBEX_HDR_BODY ||
                header_id == OBEX_HDR_BODY_END)
            {
                if (state->body)
                {
                    free(state->body);
                }
                
                state->body = (char*)malloc(hlen);
                state->body_len = hlen;
                memcpy(state->body, hdata.bs, hlen);
                
                break;
            }
        }
        
        break;
    }

    state->req_done++;
}

static void
obex_event(obex_t* hdl, obex_object_t* obj, int mode, int event,
           int obex_cmd, int obex_rsp)
{
    struct obex_state* state;
    obex_headerdata_t hd;

    state = (obex_state*)OBEX_GetUserData(hdl);

    if (event == OBEX_EV_PROGRESS) {
        hd.bq4 = state->connid;
        const int size = 4;
        const unsigned int flags = 0;
        const int rc = OBEX_ObjectAddHeader(hdl, obj, OBEX_HDR_CONNECTION,
                                            hd, size, flags);
        
        if (rc < 0)
        {
            std::cerr << "oah fail " << rc << std::endl;
        }
    }
    else if (obex_rsp != OBEX_RSP_SUCCESS && obex_rsp != OBEX_RSP_CONTINUE)
    {
        std::cerr << "FAIL " << obex_rsp << " " << event << std::endl;
//        assert(0);
        state->req_done++;
    }
    else
    {
        switch (event)
        {
        case OBEX_EV_REQDONE:
            obex_requestdone(state, hdl, obj, obex_cmd, obex_rsp);
            break;
        default:
            std::cout << "Funny event" << std::endl;
        }
    }
}

static void
pen_reset(short vendor, short product)
{
    libusb_context *ctx;
    libusb_device_handle *dev;
    int rc;

//  printf("swizzle\n");

    rc = libusb_init(&ctx);
    assert(rc == 0);
    dev = libusb_open_device_with_vid_pid(ctx, vendor, product);
    libusb_reset_device(dev);
    libusb_close(dev);
    libusb_exit(ctx);
}

static void
swizzle_usb(short vendor, short product)
{
    libusb_context* ctx;
    libusb_device_handle* dev;
    int rc;

//  printf("swizzle\n");

    rc = libusb_init(&ctx);
    assert(rc == 0);
    dev = libusb_open_device_with_vid_pid(ctx, vendor, product);
    assert(dev);

    libusb_set_configuration(dev, 1);
    libusb_set_interface_alt_setting(dev, 1, 0);
    libusb_set_interface_alt_setting(dev, 1, 1);

    libusb_close(dev);
    libusb_exit(ctx);
}

Smartpen*
Smartpen::connect(short vendor, short product)
{
    obex_t* handle;
    obex_object_t* obj;
    int rc, num, i;
    struct obex_state* state;
    obex_interface_t* obex_intf;
    obex_headerdata_t hd;
    int size, count;
    Smartpen* smartpen;

    while (true)
    {
        handle = OBEX_Init(OBEX_TRANS_USB, obex_event, 0);
        
        if (!handle)
        {
            return smartpen;
        }

        num = OBEX_FindInterfaces(handle, &obex_intf);
        
        for (i=0; i<num; i++)
        {
            if (!strcmp(obex_intf[i].usb.manufacturer, "Livescribe"))
            {
                break;
            }
        }

        if (i == num)
        {
            std::cerr << "No such device" << std::endl;
            
            handle = NULL;
            
            return smartpen;
        }

        state = (obex_state*)malloc(sizeof(struct obex_state));
        
        if (!state)
        {
            handle = NULL;
            
            return smartpen;
        }
        
        memset(state, 0, sizeof(struct obex_state));
        swizzle_usb(vendor, product);

        rc = OBEX_InterfaceConnect(handle, &obex_intf[i]);
        
        if (rc < 0)
        {
            std::cerr << "Sorry! Connecting to your device failed. "
                      << "Miserable. Is it already in use?" << std::endl;
            std::cerr << "Connect failed: " << rc << std::endl;
            
            handle = NULL;
            
            return smartpen;
        }

        OBEX_SetUserData(handle, state);
        OBEX_SetTransportMTU(handle, 0x400, 0x400);

        obj = OBEX_ObjectNew(handle, OBEX_CMD_CONNECT);
        hd.bs = (unsigned char *)"LivescribeService";
        size = strlen((char*)hd.bs)+1;
        
        OBEX_ObjectAddHeader(handle, obj, OBEX_HDR_TARGET, hd, size, 0);

        rc = OBEX_Request(handle, obj);
        count = state->req_done;
        
        while (rc == 0 && state->req_done <= count)
        {
            OBEX_HandleInput(handle, 100);
        }

        if (rc < 0 || !state->got_connid)
        {
            std::clog << "Retry connection..." << std::endl;
            
            OBEX_Cleanup(handle);
            
            continue;
        }

        smartpen = new Smartpen(handle);

        const char* buf = smartpen->getNamedObject("ppdata?key=pp0000", &rc);
        
        if (!buf)
        {
            std::clog << "Retry connection..." << std::endl;
            
            OBEX_Cleanup(handle);
            pen_reset(vendor, product);
            
            delete smartpen;
            
            continue;
        }
        
        break;
    }
    
    return smartpen;
}

const char*
Smartpen::getNamedObject(const char* name, int* len)
{
    struct obex_state *state;
    int req_done;
    obex_object_t *obj;
    obex_headerdata_t hd;
    int size, i;
    glong num;
    
    std::cout << "attempting to retrieve named object \"" << name << "\"..."
              << std::endl;
    
    state = (obex_state*)OBEX_GetUserData(this->handle);
    
    OBEX_SetTransportMTU(this->handle, OBEX_MAXIMUM_MTU, OBEX_MAXIMUM_MTU);
    
    obj = OBEX_ObjectNew(this->handle, OBEX_CMD_GET);
    size = 4;
    
//  printf("Setting connection id header to state->connid (%d)\n",state->connid);
    
    hd.bq4 = state->connid;
    
//  printf("Adding connection id header...\n");
    
    OBEX_ObjectAddHeader(handle, obj, OBEX_HDR_CONNECTION,
                         hd, size, OBEX_FL_FIT_ONE_PACKET);
//    printf("converting name from utf8 to utf16\n");
    
    hd.bs = (unsigned char *)g_utf8_to_utf16(name, strlen(name),
                                             NULL, &num, NULL);

    for (i=0; i<num; i++)
    {
        uint16_t *wchar = (uint16_t*)&hd.bs[i * 2];
        *wchar = ntohs(*wchar);
    }
    
    size = (num + 1) * sizeof(uint16_t);
    
//	printf("Adding name header...\n");
    OBEX_ObjectAddHeader(handle, obj, OBEX_HDR_NAME, hd, size, OBEX_FL_FIT_ONE_PACKET);

    if (OBEX_Request(handle, obj) < 0)
    {
        OBEX_ObjectDelete(handle,obj);
        
        std::cerr << "an error occured while retrieving the object. "
                  << "returning null value." << std::endl;
        
        return NULL;
    }

    req_done = state->req_done;
    
    while (state->req_done == req_done)
    {
        OBEX_HandleInput(handle, 100);
    }
//    printf("done handling input\n");
    if (state->body)
    {
        *len = state->body_len;
        state->body[state->body_len] = '\0';
    }
    else
    {
        *len = 0;
    }
    
    return state->body;
}

bool
Smartpen::putNamedObject(const char* name, const char* body)
{
    struct obex_state *state;
    int req_done;
    obex_object_t *obj;
    obex_headerdata_t hd;
    int name_size, body_size, i;
    glong nnum;
    glong bnum;
    
    //reference: http://dev.zuckschwerdt.org/openobex/doxygen/
    std::clog << "Attempting to set \"" << name << "\" to \"" << body << "\""
              << std::endl;
    std::clog << "getting obex state..." << std::endl;
    
    state = (obex_state*)OBEX_GetUserData(this->handle);
    
    OBEX_SetTransportMTU(this->handle, OBEX_MAXIMUM_MTU, OBEX_MAXIMUM_MTU);
    
    std::clog << "creating object" << std::endl;
    
    obj = OBEX_ObjectNew(this->handle, OBEX_CMD_PUT);
    
    if(obj == NULL)
    {
        return false;
    }
    
    body_size = strlen(body);
    
    std::clog << "determining body size: " << body_size << std::endl;
    std::clog << "Adding length header..." << std::endl;
    
    /* Add length header */
    hd.bq4 = body_size;
    
    std::cout << "length: " << body_size << std::endl;
    
    OBEX_ObjectAddHeader(handle, obj, OBEX_HDR_LENGTH, hd, 4,
                         OBEX_FL_FIT_ONE_PACKET);
    
    std::clog << "Setting connection id header to state->connid ("
              << state->connid << ")" << std::endl;
    
    hd.bq4 = state->connid;
    
    std::clog << "Adding connection id header..." << std::endl;
    
    OBEX_ObjectAddHeader(handle, obj, OBEX_HDR_CONNECTION,
                         hd, 4, OBEX_FL_FIT_ONE_PACKET);
    
    std::clog << "Adding unicode name header..." << std::endl;
    
    /* Add unicode name header*/
    hd.bs = (unsigned char *)g_utf8_to_utf16(name, strlen(name),
                                             NULL, &nnum, NULL);
    
    for (i = 0; i < nnum; i++)
    {
        uint16_t *wchar = (uint16_t*)&hd.bs[i * 2];
        *wchar = ntohs(*wchar);
    }
    
    name_size = (nnum + 1) * sizeof(uint16_t);
    
    std::cout << "name size: " << name_size << std::endl;
    
    OBEX_ObjectAddHeader(handle, obj, OBEX_HDR_NAME, hd, name_size,
                         OBEX_FL_FIT_ONE_PACKET);
    
    std::clog << "Adding body header..." << std::endl;
        
    /* Add body header*/
    hd.bs = (unsigned char*)body;
    
    std::cout << "body size: " << body_size << std::endl;
    
    OBEX_ObjectAddHeader(handle, obj, OBEX_HDR_BODY, hd, body_size, OBEX_FL_FIT_ONE_PACKET);
    
    std::clog << "Sending request..." << std::endl;
    
    return (OBEX_Request(handle, obj) >= 0);
}

const char* Smartpen::getChangeList(int startTime) {
    char name[256];
    int len;
    
    //FIXME Reimplement as stream w/ insertion operator?
    snprintf(name, sizeof(name), "changelist?start_time=%d", startTime);
    return getNamedObject(name, &len);
}

void
Smartpen::disconnect() {
    struct obex_state *state;
    int req_done;
    obex_object_t *obj;
    obex_headerdata_t hd;
    int size;

    state = (obex_state*)OBEX_GetUserData(handle);
    obj = OBEX_ObjectNew(handle, OBEX_CMD_DISCONNECT);
    hd.bq4 = state->connid;
    size = 4;
    
    OBEX_ObjectAddHeader(handle, obj, OBEX_HDR_CONNECTION,
        hd, size, OBEX_FL_FIT_ONE_PACKET);

    if (OBEX_Request(handle, obj) >= 0)
    {
        req_done = state->req_done;
        while (state->req_done == req_done)
        {
            OBEX_HandleInput(handle, 100);
        }
        
        handle = NULL;
        
        std::clog << "disconnected." << std::endl;
    }
}

int
Smartpen::getGuid(FILE* out, const char* guid, long long int startTime)
{
    char name[256];
    int len;
    
    //FIXME Reimplement as stream w/ insertion operator?
    snprintf(name, sizeof(name), "lspdata?name=%s&start_time=%lld",
             guid, startTime);

    const char* buf = getNamedObject(name, &len);
    
    //FIXME Reimplement as stream w/ insertion operator?
    fwrite(buf, len, 1, out);
    return len;
}

//int Smartpen::getPaperReplay(FILE *out, long long int startTime)
//{
//  char name[256];
//  char *buf;
//  int len;
//
//  snprintf(name, sizeof(name), "lspdata?name=com.livescribe.paperreplay.PaperReplay&start_time=%lld&returnVersion=0.3&remoteCaller=WIN_LD_200",
//      startTime);
//
//  buf = getNamedObject(name, &len);
//  fwrite(buf, len, 1, out);
//  return 1;
//}

const char*
Smartpen::getPaperReplay(long long int startTime)
{
    char name[256];
    char *buf;
    int len;
    
    //FIXME Reimplement as stream with insertion operator?
    snprintf(name, sizeof(name), "lspdata?name=com.livescribe.paperreplay.PaperReplay&start_time=%lld&returnVersion=0.3&remoteCaller=WIN_LD_200", startTime);

    return getNamedObject(name, &len);
}

const char*
Smartpen::getPenletList()
{
    const char *name = "penletlist";
    int len;

    return getNamedObject(name, &len);
}


std::string
to_hex(const std::string& src, bool spaces)
{
    //source: http://www.programmingforums.org/thread14348.html
    std::stringstream out;
    
    for (std::string::size_type i = 0; i < src.size(); i++)
    {
        out<< std::hex << int ( src[i] );
        
        if ((i < src.size() - 1) && (spaces))
        {
            out<<' ';
        }
    }
    
    return out.str();
}

std::string
from_hex(const std::string& src)
{
    //source: http://www.programmingforums.org/thread14348.html
    std::stringstream in(src);
    std::string ret;
    int byte;
    
    while (in>> std::hex >> byte)
    {
        ret += char ( byte );
    }
    
    return ret;
}

std::string
space_hex(const std::string& src)
{
    std::string ret;
    
    for(unsigned int x = 0; x < src.length(); x += 2)
    {
        ret += src.substr(x,2) + ' ';
    }
    
    return ret;
}

bool
get_parameter_exists(const char * xml)
{
    xmlDocPtr doc = xmlParseMemory(xml, strlen(xml));
    xmlNodePtr cur = xmlDocGetRootElement(doc); //current element should be "xml" at this point.
    
    if (cur == NULL)
    {
        std::cout << "cur is NULL!" << std::endl;
        
        xmlFreeDoc(doc);
        
        return ""; //do nothing if the xml document is empty
    }
    
    if ((xmlStrcmp(cur->name, (const xmlChar *)"xml")) != 0)
    {
        return ""; //do nothing if the current element's name is not 'xml'
    }
    
    cur = cur->children;
    
    for (cur = cur; cur; cur = cur->next)
    {
        if (cur->type == XML_ELEMENT_NODE)
        {
            if ((!xmlStrcmp(cur->name, (const xmlChar *)"parameter"))) //if the current element's name is 'parameter'
            {
                const xmlChar* value = xmlGetProp(cur, (const xmlChar*)"exists");
                
                return ((!xmlStrcmp(value, (const xmlChar *)"true"))); //return whether exists is true
            }
        }
    }
    
    assert(false);
    
    return false;
}

const char*
get_parameter_value(const char * xml)
{
    xmlDocPtr doc = xmlParseMemory(xml, strlen(xml));
    xmlNodePtr cur = xmlDocGetRootElement(doc); //current element should be "xml" at this point.
    
    if (cur == NULL)
    {
        std::cout << "cur is NULL!" << std::endl;
        xmlFreeDoc(doc);
        
        return ""; //do nothing if the xml document is empty
    }
    
    if ((xmlStrcmp(cur->name, (const xmlChar *)"xml")) != 0)
    {
        return ""; //do nothing if the current element's name is not 'xml'
    }
    
    cur = cur->children;
    
    for (cur = cur; cur; cur = cur->next)
    {
        if (cur->type == XML_ELEMENT_NODE)
        {
            if ((!xmlStrcmp(cur->name, (const xmlChar *)"parameter"))) //if the current element's name is 'parameter'
            {
                char* value = (char*)xmlGetProp(cur, (const xmlChar*)"value");
                return value;
            }
        }
    }
    
    assert(false);
    
    return 0;
}

const char*
Smartpen::getName()
{
    const char* name = "ppdata?key=pp8011";
    int len;
    const char* retrieved = getNamedObject(name, &len);
    
    if (retrieved != NULL)
    {
        std::clog << "retrieved: " << retrieved << std::endl;
        
        if (get_parameter_exists(retrieved))
        {
            const char* value = get_parameter_value(retrieved);
            std::string dN = value;
            dN = dN.substr(2);
            
            std::cout << "device name (hex): " << dN.c_str() << std::endl;
            std::cout << "From HEX: " << from_hex(space_hex(dN)).c_str()
                      << std::endl;
            
            return from_hex(space_hex(dN)).c_str();
        }
        else
        {
            std::clog << "device name is not set." << std::endl;
            
            return "(Unnamed Smartpen)";
        }
    }
    
    assert(false);
    
    return 0;
}

bool
Smartpen::setName(const char* newName)
{
    const char* name_header = "ppdata?key=pp8011";
    bool success = putNamedObject(name_header, newName);
    
    std::clog << "Attempting to set smartpen name to \"" << newName
              << "\"..." << std::endl;
    
    if (success)
    {
        std::clog << "Name should have been changed successfully"
                  << std::endl;
    }
    else
    {
        std::cerr << "Error occured while changing smartpen name."
                  << std::endl;
    }
    
    return success;
}

const char*
Smartpen::getInfo()
{
    const char* name = "peninfo";
    int len;

    return getNamedObject(name, &len);
}

const char*
Smartpen::getCertificate()
{
    const char* name = "ppdata?key=pp8010";
    int len;

    const char* retrieved = getNamedObject(name, &len);
    
    if (retrieved != NULL)
    {
        const char* value = get_parameter_value(retrieved);
        std::string hexCert = value;
        hexCert = hexCert.substr(2);
        
//        std::cout << "Certificate (hex): " << hexCert.c_str() << std::endl;
        std::cout << "From HEX: " << from_hex(space_hex(hexCert)).c_str()
                  << std::endl;
        
        return from_hex(space_hex(hexCert)).c_str();
    }
    
    assert(false);
    
    return 0;
}

const char*
Smartpen::getSessionList ()
{
    const char* name = "lspcommand?name=Paper Replay&command=listSessions";
    int len;

    return getNamedObject(name, &len);
}

bool
Smartpen::resetPassword()
{
    const char* name = "lspcommand?name=Paper Replay&command=resetPW";
    int len;
    const char* result = getNamedObject(name, &len);
    
    std::clog << "resetting paper replay password: " << result << std::endl;
    
    return (strcmp("success",result) == 0); //returns whether or not we successfully reset the password
}

void
Smartpen::getLspData(const char* object_name, long long int start_time)
{
    char name[256];
    int len;
    
    snprintf(name, sizeof(name), "lspdata?name=%s&start_time=%d",object_name,int(start_time));
    
    char* loc = (char*)malloc(snprintf(NULL, 0, "%s%s", "./data/", object_name) + 1);
    sprintf(loc, "%s%s", "./data/", object_name);
    
    if (FILE * file = fopen(loc, "r"))
    {
        fclose(file); //the file already exists
    }
    else
    {
        std::clog << "Downloading object with guid " << object_name
                  << " from smartpen..." << std::endl;
        
        const char* buf = getNamedObject(name, &len);
        
        FILE* out = fopen(loc, "w");
        fwrite(buf, len, 1, out);
        fclose(out);
        
        //FIXME IMPERATIVE that we implement zlib or other compression library.
        std::string cmd = "unzip -qq -o -d ./data/extracted/" + (std::string)object_name + " ./data/" + (std::string)object_name;
        
        system(cmd.c_str()); //I know... this is a horrible way to unzip the files, but it's so easy!
    }
}

