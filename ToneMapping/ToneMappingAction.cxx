#include <dzapp.h>
#include <dzmainwindow.h>

#include "TonemappingAction.h"
#include "TonemappingDialog.h"

ToneMappingAction::ToneMappingAction() :
	DzAction("Tone Mapping", "Custom Tone Mapping")
{
	_dlg = NULL;
}

void
ToneMappingAction::executeAction()
{
	DzMainWindow *wnd = dzApp->getInterface();
	if (!wnd)
		return;

	if (!_dlg)
		_dlg = new ToneMappingDialog(wnd);

	_dlg->show();
}

ToneMappingAction::~ToneMappingAction()
{
	delete _dlg;
}