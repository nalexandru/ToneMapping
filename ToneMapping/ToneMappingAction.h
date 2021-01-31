#ifndef _TONEMAPPING_ACTION_H_
#define _TONEMAPPING_ACTION_H_

#include <dzaction.h>

class ToneMappingDialog;

class ToneMappingAction : public DzAction {
	Q_OBJECT
public:

	ToneMappingAction();

	virtual QString getDefaultMenuPath() const { return tr("&Render"); }

	virtual ~ToneMappingAction();
protected:
	virtual void executeAction();

private:
	ToneMappingDialog *_dlg;
};

#endif /* _TONEMAPPING_ACTION_H_ */
