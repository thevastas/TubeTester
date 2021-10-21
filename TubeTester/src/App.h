#pragma once
#include "Common.h"

class App : public wxApp
{
public:
	App();
	bool OnInit();
	~App();
};

DECLARE_APP(App);
