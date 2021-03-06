// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license.

#pragma once

#include <napa/module/binding.h>
#include <napa/transport/transport-context.h>
#include <napa/v8-helpers.h>

namespace napa {
namespace transport {

    /// <summary> Register a Transportable object wrap with transport. </summary>
    /// <param name="constructor"> Constructor of wrap type. </param>
    /// <remarks> 'napajs/lib/transport/transport' is required instead of 'napajs/lib/transport' to avoid circular dependency on addon. </remarks>
    inline void Register(v8::Local<v8::Function> constructor) {
        v8::Local<v8::Value> argv[] = { constructor };
        (void)napa::module::binding::Call("../lib/transport/transport", "register", sizeof(argv) / sizeof(v8::Local<v8::Value>), argv);
    }

    /// <summary> Marshall an object with transport context. C++ modules can use this helper function to marshall its members. </summary>
    /// <param name="object"> Object to marshall, it can be built-in JavaScript types or object implements napajs.transport.Transportable. </param>
    /// <param name="transportContextWrap"> TransportContextWrap to save shareable states if any. </param>
    /// <returns> Payload in V8 string of marshalled object. </summary>
    /// <remarks> 'napajs/lib/transport/transport' is required instead of 'napajs/lib/transport' to avoid circular dependency on addon. </remarks>
    inline v8::MaybeLocal<v8::String> Marshall(v8::Local<v8::Value> object, v8::Local<v8::Object> transportContextWrap) {
        v8::Local<v8::Value> argv[] = { object, transportContextWrap };
        return v8_helpers::MaybeCast<v8::String>(napa::module::binding::Call(
            "../lib/transport/transport", 
            "marshall", 
            sizeof(argv) / sizeof(v8::Local<v8::Value>), 
            argv));
    }

    /// <summary> Marshall an object with transport context. C++ modules can use this helper function to marshall its members. </summary>
    /// <param name="object"> Object to marshall, it can be built-in JavaScript types or object implements napajs.transport.Transportable. </param>
    /// <param name="transportContext"> TransportContext to save shareable states if any. </param>
    /// <returns> Payload in V8 string of marshalled object. </summary>
    /// <remarks> 'napajs/lib/transport/transport' is required instead of 'napajs/lib/transport' to avoid circular dependency on addon. </remarks>
    inline v8::MaybeLocal<v8::String> Marshall(v8::Local<v8::Value> object, napa::transport::TransportContext* transportContext) {
        auto isolate = v8::Isolate::GetCurrent();
        v8::Local<v8::Value> argv[] = { 
            v8::Boolean::New(isolate, false),                           // Not owning since wrap is temporary.
            v8_helpers::PtrToV8Uint32Array(isolate, transportContext) 
        };
        auto transportContextWrap = napa::module::binding::NewInstance(
            "TransportContextWrap", 
            sizeof(argv) / sizeof(v8::Local<v8::Value>), 
            argv).ToLocalChecked();

        return Marshall(object, transportContextWrap);
    }

    /// <summary> Unmarshall a payload with transport context. C++ modules can use this helper function to unmarshall its members. </summary>
    /// <param name="payload"> Payload to unmarshall, it is plain JS object generated by transport.marshallSingle. </param>
    /// <param name="transportContextWrap"> TransportContextWrap to load shareable states if any. </param>
    /// <returns> Unmarshalled V8 value from payload. </summary>
    /// <remarks> 'napajs/lib/transport/transport' is required instead of 'napajs/lib/transport' to avoid circular dependency on addon. </remarks>
    inline v8::MaybeLocal<v8::Value> Unmarshall(v8::Local<v8::Value> payload, v8::Local<v8::Object> transportContextWrap) {
        v8::Local<v8::Value> argv[] = { payload, transportContextWrap };
        return napa::module::binding::Call("../lib/transport/transport", "unmarshall", sizeof(argv) / sizeof(v8::Local<v8::Value>), argv);
    }

    /// <summary> Unmarshall a payload with transport context. C++ modules can use this helper function to unmarshall its members. </summary>
    /// <param name="payload"> Payload to unmarshall, it is plain JS object generated by transport.marshallSingle. </param>
    /// <param name="transportContext"> TransportContext to load shareable states if any. </param>
    /// <returns> Unmarshalled V8 value from payload. </summary>
    /// <remarks> 'napajs/lib/transport/transport' is required instead of 'napajs/lib/transport' to avoid circular dependency on addon. </remarks>
    inline v8::MaybeLocal<v8::Value> Unmarshall(v8::Local<v8::Value> payload, const napa::transport::TransportContext* transportContext) {
        auto isolate = v8::Isolate::GetCurrent();
        v8::Local<v8::Value> argv[] = { 
            v8::Boolean::New(isolate, false),                           // Not owning since wrap is temporary.
            v8_helpers::PtrToV8Uint32Array(isolate, transportContext) 
        };
        auto transportContextWrap = napa::module::binding::NewInstance(
            "TransportContextWrap", 
            sizeof(argv) / sizeof(v8::Local<v8::Value>), 
            argv).ToLocalChecked();

        return Unmarshall(payload, transportContextWrap);
    }
}
}