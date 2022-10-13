 //The following include files are necessary to allow your MyPanel.cpp to compile.
 #include "cbase.h"
 #include "IMyPanel.h"
 using namespace vgui;
 #include <vgui/IVGui.h>
 #include <vgui_controls/Frame.h>
 
 //CMyPanel class: Tutorial example class
 class CMyPanel : public vgui::Frame
 {
 	DECLARE_CLASS_SIMPLE(CMyPanel, vgui::Frame); 
 	//CMyPanel : This Class / vgui::Frame : BaseClass
 
 	CMyPanel(vgui::VPANEL parent); 	// Constructor
 	~CMyPanel(){};				// Destructor

 protected:
 	//VGUI overrides:
 	virtual void OnTick();
 	virtual void OnCommand(const char* pcCommand);
 
 private:
 	//Other used VGUI control Elements:
    // Constuctor: Initializes the Panel
 };