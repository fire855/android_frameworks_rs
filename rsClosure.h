#ifndef ANDROID_RENDERSCRIPT_CLOSURE_H_
#define ANDROID_RENDERSCRIPT_CLOSURE_H_

#include "rsDefines.h"
#include "rsMap.h"
#include "rsObjectBase.h"

namespace android {
namespace renderscript {

class Allocation;
class Context;
class ObjectBase;
class ScriptFieldID;
class ScriptInvokeID;
class ScriptKernelID;
class Type;

class Closure : public ObjectBase {
 public:
    Closure(Context* context,
            const ScriptKernelID* kernelID,
            Allocation* returnValue,
            const int numValues,
            const ScriptFieldID** fieldIDs,
            const void** values,  // Allocations or primitive (numeric) types
            const size_t* sizes,  // size for data type. -1 indicates an allocation.
            const Closure** depClosures,
            const ScriptFieldID** depFieldIDs);
    Closure(Context* context,
            const ScriptInvokeID* invokeID,
            const void* params,
            const size_t paramLength,
            const size_t numValues,
            const ScriptFieldID** fieldIDs,
            const void** values,  // Allocations or primitive (numeric) types
            const size_t* sizes);  // size for data type. -1 indicates an allocation.

    virtual ~Closure();

    virtual void serialize(Context *rsc, OStream *stream) const {}

    virtual RsA3DClassID getClassId() const { return RS_A3D_CLASS_ID_CLOSURE; }

    void setArg(const uint32_t index, const void* value, const size_t size);
    void setGlobal(const ScriptFieldID* fieldID, const void* value,
                   const size_t size);

    Context* mContext;

    // If mKernelID is not null, this is a closure for a kernel. Otherwise, it is
    // a closure for an invoke function, whose id is the next field. At least one
    // of these fields has to be non-null.
    const ObjectBaseRef<ScriptKernelID> mKernelID;
    // TODO(yangni): ObjectBaseRef<ScriptInvokeID>
    const ScriptInvokeID* mInvokeID;

    // Values referrenced in arguments and globals cannot be futures. They must be
    // either a known value or unbound value.
    // For now, all arguments should be Allocations.
    const void** mArgs;
    size_t mNumArg;

    // A global could be allocation or any primitive data type.
    Map<const ScriptFieldID*, Pair<const void*, size_t>> mGlobals;

    Allocation* mReturnValue;

    // All the other closures that this closure depends on
    // set<const Closure*> mDependences;

    // All the other closures which this closure depends on for one of its
    // arguments, and the fields which it depends on.
    Map<const Closure*, Map<int, const ObjectBaseRef<ScriptFieldID>*>*> mArgDeps;

    // All the other closures that this closure depends on for one of its fields,
    // and the fields that it depends on.
    Map<const Closure*, Map<const ObjectBaseRef<ScriptFieldID>*,
            const ObjectBaseRef<ScriptFieldID>*>*> mGlobalDeps;

    const void* mParams;
    const size_t mParamLength;
};

}  // namespace renderscript
}  // namespace android

#endif  // ANDROID_RENDERSCRIPT_CLOSURE_H_