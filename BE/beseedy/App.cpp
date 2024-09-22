/*
 * Copyright 2024, My Name <my@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "App.h"
#include "MainWindow.h"
#

const char* kApplicationSignature = "application/x-vnd.SysauLab-BeSeedy";

App::App()
	:
	BApplication(kApplicationSignature)
{
	char random[32];
	uint64_t output;
	seedy64((uint8_t*)&output, sizeof(output));
	sprintf(&random[0], "%016llx", output);
	BWindow *mywin = new BWindow(
		BRect(100, 100, 235, 135), 
		"BeSeedy", 
		B_TITLED_WINDOW, 
		B_ASYNCHRONOUS_CONTROLS | B_QUIT_ON_WINDOW_CLOSE);
	
	BStringView *sv = new BStringView(
		BRect(10, 10, 50, 40),
		"Labely", 
		&random[0], 
		0, 
		B_WILL_DRAW);
		
	sv->ResizeToPreferred();	
	mywin->AddChild(sv);
	mywin->Show();
}


App::~App()
{
}


int
main()
{
	App* app = new App();
	app->Run();
	delete app;
	return 0;
}
