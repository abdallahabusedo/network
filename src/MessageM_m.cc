//
// Generated file, do not edit! Created by nedtool 5.7 from MessageM.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include "MessageM_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp


// forward
template<typename T, typename A>
std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec);

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// operator<< for std::vector<T>
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

MessageM_Base::MessageM_Base(const char *name, short kind) : ::omnetpp::cPacket(name,kind)
{
    this->type = 0;
    this->id = 0;
    this->sendingTime = 0;
    this->mode = false;
    this->loss = false;
    this->duple = false;
    this->delay = false;
}

MessageM_Base::MessageM_Base(const MessageM_Base& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

MessageM_Base::~MessageM_Base()
{
}

MessageM_Base& MessageM_Base::operator=(const MessageM_Base& other)
{
    if (this==&other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void MessageM_Base::copy(const MessageM_Base& other)
{
    this->type = other.type;
    this->id = other.id;
    this->sendingTime = other.sendingTime;
    this->payload = other.payload;
    this->trailer = other.trailer;
    this->mode = other.mode;
    this->loss = other.loss;
    this->duple = other.duple;
    this->delay = other.delay;
}

void MessageM_Base::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->type);
    doParsimPacking(b,this->id);
    doParsimPacking(b,this->sendingTime);
    doParsimPacking(b,this->payload);
    doParsimPacking(b,this->trailer);
    doParsimPacking(b,this->mode);
    doParsimPacking(b,this->loss);
    doParsimPacking(b,this->duple);
    doParsimPacking(b,this->delay);
}

void MessageM_Base::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->type);
    doParsimUnpacking(b,this->id);
    doParsimUnpacking(b,this->sendingTime);
    doParsimUnpacking(b,this->payload);
    doParsimUnpacking(b,this->trailer);
    doParsimUnpacking(b,this->mode);
    doParsimUnpacking(b,this->loss);
    doParsimUnpacking(b,this->duple);
    doParsimUnpacking(b,this->delay);
}

int MessageM_Base::getType() const
{
    return this->type;
}

void MessageM_Base::setType(int type)
{
    this->type = type;
}

int MessageM_Base::getId() const
{
    return this->id;
}

void MessageM_Base::setId(int id)
{
    this->id = id;
}

double MessageM_Base::getSendingTime() const
{
    return this->sendingTime;
}

void MessageM_Base::setSendingTime(double sendingTime)
{
    this->sendingTime = sendingTime;
}

const char * MessageM_Base::getPayload() const
{
    return this->payload.c_str();
}

void MessageM_Base::setPayload(const char * payload)
{
    this->payload = payload;
}

bits& MessageM_Base::getTrailer()
{
    return this->trailer;
}

void MessageM_Base::setTrailer(const bits& trailer)
{
    this->trailer = trailer;
}

bool MessageM_Base::getMode() const
{
    return this->mode;
}

void MessageM_Base::setMode(bool mode)
{
    this->mode = mode;
}

bool MessageM_Base::getLoss() const
{
    return this->loss;
}

void MessageM_Base::setLoss(bool loss)
{
    this->loss = loss;
}

bool MessageM_Base::getDuple() const
{
    return this->duple;
}

void MessageM_Base::setDuple(bool duple)
{
    this->duple = duple;
}

bool MessageM_Base::getDelay() const
{
    return this->delay;
}

void MessageM_Base::setDelay(bool delay)
{
    this->delay = delay;
}

class MessageMDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    MessageMDescriptor();
    virtual ~MessageMDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(MessageMDescriptor)

MessageMDescriptor::MessageMDescriptor() : omnetpp::cClassDescriptor("MessageM", "omnetpp::cPacket")
{
    propertynames = nullptr;
}

MessageMDescriptor::~MessageMDescriptor()
{
    delete[] propertynames;
}

bool MessageMDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<MessageM_Base *>(obj)!=nullptr;
}

const char **MessageMDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = { "customize",  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *MessageMDescriptor::getProperty(const char *propertyname) const
{
    if (!strcmp(propertyname,"customize")) return "true";
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int MessageMDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 9+basedesc->getFieldCount() : 9;
}

unsigned int MessageMDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<9) ? fieldTypeFlags[field] : 0;
}

const char *MessageMDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "type",
        "id",
        "sendingTime",
        "payload",
        "trailer",
        "mode",
        "loss",
        "duple",
        "delay",
    };
    return (field>=0 && field<9) ? fieldNames[field] : nullptr;
}

int MessageMDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='t' && strcmp(fieldName, "type")==0) return base+0;
    if (fieldName[0]=='i' && strcmp(fieldName, "id")==0) return base+1;
    if (fieldName[0]=='s' && strcmp(fieldName, "sendingTime")==0) return base+2;
    if (fieldName[0]=='p' && strcmp(fieldName, "payload")==0) return base+3;
    if (fieldName[0]=='t' && strcmp(fieldName, "trailer")==0) return base+4;
    if (fieldName[0]=='m' && strcmp(fieldName, "mode")==0) return base+5;
    if (fieldName[0]=='l' && strcmp(fieldName, "loss")==0) return base+6;
    if (fieldName[0]=='d' && strcmp(fieldName, "duple")==0) return base+7;
    if (fieldName[0]=='d' && strcmp(fieldName, "delay")==0) return base+8;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *MessageMDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "double",
        "string",
        "bits",
        "bool",
        "bool",
        "bool",
        "bool",
    };
    return (field>=0 && field<9) ? fieldTypeStrings[field] : nullptr;
}

const char **MessageMDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *MessageMDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int MessageMDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    MessageM_Base *pp = (MessageM_Base *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *MessageMDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    MessageM_Base *pp = (MessageM_Base *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string MessageMDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    MessageM_Base *pp = (MessageM_Base *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getType());
        case 1: return long2string(pp->getId());
        case 2: return double2string(pp->getSendingTime());
        case 3: return oppstring2string(pp->getPayload());
        case 4: return pp->getTrailer().to_string();
        case 5: return bool2string(pp->getMode());
        case 6: return bool2string(pp->getLoss());
        case 7: return bool2string(pp->getDuple());
        case 8: return bool2string(pp->getDelay());
        default: return "";
    }
}

bool MessageMDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    MessageM_Base *pp = (MessageM_Base *)object; (void)pp;
    switch (field) {
        case 0: pp->setType(string2long(value)); return true;
        case 1: pp->setId(string2long(value)); return true;
        case 2: pp->setSendingTime(string2double(value)); return true;
        case 3: pp->setPayload((value)); return true;
        case 5: pp->setMode(string2bool(value)); return true;
        case 6: pp->setLoss(string2bool(value)); return true;
        case 7: pp->setDuple(string2bool(value)); return true;
        case 8: pp->setDelay(string2bool(value)); return true;
        default: return false;
    }
}

const char *MessageMDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 4: return omnetpp::opp_typename(typeid(bits));
        default: return nullptr;
    };
}

void *MessageMDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    MessageM_Base *pp = (MessageM_Base *)object; (void)pp;
    switch (field) {
        case 4: return (void *)(&pp->getTrailer()); break;
        default: return nullptr;
    }
}


