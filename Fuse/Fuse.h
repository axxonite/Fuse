// Fuse.h

#pragma once

#define DIRECTINPUT_VERSION         0x0800

#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <string>
#include <wrl.h>
#include <shellapi.h>
#include <d3d9.h>
#include <dinput.h>
#include <math.h>
#include <typeinfo.h>
#include <windows.h>

using namespace std;
using namespace Microsoft::WRL;
using namespace DirectX;

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