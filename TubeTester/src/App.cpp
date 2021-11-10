#define WIN32_LEAN_AND_MEAN
#include "App.h"
#include "id.h"
#include "MainWindow.h"

INITIALIZE_EASYLOGGINGPP

App::App()
{

}

bool App::OnInit()
{
	if (!wxApp::OnInit())
		return false;
	cv::setBreakOnError(true);
	MainWindow* main = new MainWindow(nullptr, window::id::MAINWINDOW, _("Main Window"), wxDefaultPosition, wxSize(1300, 1300));
	main->Show();
	return true;
}

App::~App()
{

}
IMPLEMENT_APP(App);