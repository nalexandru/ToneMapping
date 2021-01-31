#include <dzplugin.h>
#include <dzapp.h>

#include "Tonemapping.h"
#include "TonemappingAction.h"

class ToneMappingPlugin : public DzPlugin
{
public:
	ToneMappingPlugin() : DzPlugin("ToneMapping")
	{
		initToneMap();
	}

	virtual ~ToneMappingPlugin()
	{
		termToneMap();
	}
};

DZ_CUSTOM_PLUGIN_DEFINITION(ToneMappingPlugin);
DZ_PLUGIN_AUTHOR("Alexandru Naiman");
DZ_PLUGIN_VERSION(0, 1, 0, 1);
DZ_PLUGIN_DESCRIPTION(QString(""));

DZ_PLUGIN_CLASS_GUID(ToneMappingAction, 3a6f028b-a354-4906-b3e0-ec5bd111b944);
