// Fuse.h

#pragma once

#include <vector>
#include <list>
#include <map>
#include <algorithm>

using namespace std;

#define DIRECTINPUT_VERSION         0x0800

#include <d3d9.h>
#include <dinput.h>
#include <math.h>
#include <typeinfo.h>

#include "assert.h"
#include "Types.h"
#include "Color.h"
#include "Constants.h"
#include "Macros.h"
#include "Exceptions.h"

#include "Vector.h"
#include "Vector2.h"
#include "Vector4.h"
#include "Matrix3.h"
#include "Matrix4.h"
#include "Plane.h"
#include "Quaternion.h"
#include "Sphere.h"

#include "BaseObject.h"
#include "Trace.h"