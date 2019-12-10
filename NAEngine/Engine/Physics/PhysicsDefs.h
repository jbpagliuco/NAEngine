#pragma once

#define NA_PHYSICS_LOG_FILTER "Physics"

#define NA_PHYSICS_SAFE_RELEASE(x) if ((x)) { (x)->release(); (x) = nullptr; }