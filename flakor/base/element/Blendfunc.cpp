#include "targetMacros.h"
#include "base/element/Blendfunc.h"

FLAKOR_NS_BEGIN

const BlendFunc BlendFunc::DISABLE = {GL_ONE, GL_ZERO};
const BlendFunc BlendFunc::ALPHA_PREMULTIPLIED = {GL_ONE, GL_ONE_MINUS_SRC_ALPHA};
const BlendFunc BlendFunc::ALPHA_NON_PREMULTIPLIED = {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA};
const BlendFunc BlendFunc::ADDITIVE = {GL_SRC_ALPHA, GL_ONE};

FLAKOR_NS_END