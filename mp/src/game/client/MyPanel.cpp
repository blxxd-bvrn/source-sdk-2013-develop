//The following include files are necessary to allow your MyPanel.cpp to compile.
//I copied like a lot of code from the documentation lmao, I'm lazy as shit
#include "cbase.h"
#include "KeyValues.h"
#include "filesystem.h"
#include "IMyPanel.h"
#include "ienginevgui.h"
using namespace vgui;
#include <vgui/IVGui.h>
#include "vgui/IInput.h"
#include "vgui/ILocalize.h"
#include <vgui_controls/Frame.h>
#include "vgui_controls/Menu.h"
#include <vgui_controls/Button.h>
#include <vgui_controls/ListPanel.h>
#include "vgui_controls/FileOpenDialog.h"
#include <vgui_controls/DirectorySelectDialog.h>
#include "checksum_crc.h"

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
	Button *m_pCloseButton;
	Button *m_pAcceptButton; //Goddamnit Anthony
	ListPanel *m_mapPanel;
public:
	void FindMaps() {
		KeyValues *kv = new KeyValues("MAP");
		FileFindHandle_t findHandle = NULL;
		const char *pszFilename = g_pFullFileSystem->FindFirst( "maps/*.bsp", &findHandle );
		while (pszFilename)
		{
			char mapname[256];
			const char *str = Q_strstr( pszFilename, "maps" );
			if ( str )
			{
				Q_strncpy( mapname, str + 5, sizeof(mapname) - 1 );
			}
			else
			{
				Q_strncpy( mapname, pszFilename, sizeof(mapname) - 1 );
			}
			char *ext = Q_strstr( mapname, ".bsp" );
			if ( ext )
			{
				*ext = 0;
			}
			//Q_snprintf( mapname, sizeof(mapname), "maps/%s", pszFilename );
			kv->SetString("File", mapname);
			m_mapPanel->AddItem(kv, 0, false, false);
			pszFilename = g_pFullFileSystem->FindNext(findHandle);
		}
		kv->deleteThis();
		g_pFullFileSystem->FindClose(findHandle);
	}
};

// Constuctor: Initializes the Panel

CMyPanel::CMyPanel(vgui::VPANEL parent)
	: BaseClass(NULL, "MyPanel")
{
	SetParent(parent);
	SetKeyBoardInputEnabled(true);
	SetMouseInputEnabled(true);
	SetProportional(false);
	SetTitleBarVisible(true);
	SetMinimizeButtonVisible(false);
	SetMaximizeButtonVisible(false);
	SetCloseButtonVisible(false);
	SetSizeable(false);
	SetMoveable(false);
	SetVisible(true);


	SetScheme(vgui::scheme()->LoadSchemeFromFile("resource/SourceScheme.res", "SourceScheme"));
	LoadControlSettings("resource/UI/mypanel.res");
	vgui::ivgui()->AddTickSignal(GetVPanel(), 100);
	
	DevMsg("MyPanel has been constructed\n");
	
	m_mapPanel = new ListPanel(this, "MapPanel");
	m_mapPanel->SetPos(16, 32);
	m_mapPanel->SetSize(482, 428);
	m_mapPanel->AddColumnHeader( 0, "File", "Maps", 200, ListPanel::COLUMN_RESIZEWITHWINDOW);
	//Another button done
	m_pAcceptButton = new Button(this, "AcceptButton", "Load Map", this, "map&mapname");
	//Button done
	m_pCloseButton = new Button(this, "CloseButton", "Close", this, "turnoff");
	m_pCloseButton->SetPos(433, 472);
	m_pAcceptButton->SetPos(356, 472);
	m_pAcceptButton->SetSize(70, 24);
	
	m_pAcceptButton->SetDepressedSound("common/bugreporter_succeeded.wav");
	m_pAcceptButton->SetReleasedSound("ui/buttonclick.wav");
	FindMaps();
}

//Class: CMyPanelInterface Class. Used for construction.
class CMyPanelInterface : public MyPanel
{
private:
	CMyPanel *MyPanel;
public:
	CMyPanelInterface()
	{
		MyPanel = NULL;
	}


	void Create(vgui::VPANEL parent)
	{
		MyPanel = new CMyPanel(parent);
	}
	void Destroy()
	{
		if (MyPanel)
		{
			MyPanel->SetParent((vgui::Panel *)NULL);
			delete MyPanel;
		}
	}
	void Activate(void)
	{
		if (MyPanel)
		{
			MyPanel->Activate();
		}
	}
};

static CMyPanelInterface g_MyPanel;
MyPanel* mypanel = (MyPanel*)&g_MyPanel;

ConVar cl_showmypanel("cl_showmypanel", "0", FCVAR_CLIENTDLL, "Sets the state of myPanel <state>");

void CMyPanel::OnTick()
{
	BaseClass::OnTick();
	SetVisible(cl_showmypanel.GetBool());
}

CON_COMMAND(ToggleMyPanel, "Toggles myPanel on or off")
{
	cl_showmypanel.SetValue(!cl_showmypanel.GetBool());
	mypanel->Activate();
};

void CMyPanel::OnCommand(const char* pcCommand)
{
	BaseClass::OnCommand(pcCommand);
	if (!Q_stricmp(pcCommand, "turnoff"))
		cl_showmypanel.SetValue(0);

	if (!Q_stricmp(pcCommand, "map&mapname")) {
		cl_showmypanel.SetValue(0);
		int selCount = m_mapPanel->GetSelectedItemsCount();
        if ( selCount <= 0 )
        {
            return;
        }
        for ( int i = 0; i < selCount; ++i )
        {
			int itemId = m_mapPanel->GetSelectedItem(0);
			KeyValues *kv = m_mapPanel->GetItem(itemId);
            if ( !kv )
            {
                continue;
            }
			const char *map = kv->GetString("File", "&");
            if ( (map = "&") != 0) {
                continue;
            }
        }
		DevMsg("Map has been loaded! %s", selCount);
	}
}

