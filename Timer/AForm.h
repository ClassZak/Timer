#pragma once
#include <Windows.h>
#include <list>
#include <map>
#include <string>
#include <functional>

namespace DeclarativeClasses
{
class AForm
{
public:
	virtual void SetResizeMethod(const std::function<BOOL(int, int, void*)>& function)=0;
	virtual void Resize()=0;
	virtual void SetNewSize(int w, int h)=0;
};
}


