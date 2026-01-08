#pragma once

#include "glm/ext/matrix_float2x2.hpp"
#include "glm/ext/matrix_float3x3.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"

namespace tengine {
    
//! Математические фунции, доступные внутри движка.
//! 
//! На данный момент это просто копия GLM,
//! и является заделкой на будущее.
namespace math {

using namespace glm;

}
} // namespace tengine
