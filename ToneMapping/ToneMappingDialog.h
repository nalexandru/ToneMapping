#ifndef _TONEMAPPING_DIALOG_H_
#define _TONEMAPPING_DIALOG_H_

#include <stdint.h>

#include <QtGui/qlabel.h>
#include <QtGui/qimage.h>

#include <dzbasicdialog.h>

class ToneMappingDialog : public DzBasicDialog
{
	Q_OBJECT

public:
	ToneMappingDialog(QWidget *parent = 0);

	virtual ~ToneMappingDialog();

public slots:
	void loadImage();
	void saveImage();

	void exposureChanged(float val);
	void gammaChanged(float val);
	void mtStateChanged(int state);
	void modeChanged(int mode);

private:
	QLabel *_imageView;
	enum ToneMapMode _mode;
	int _imgWidth, _imgHeight;
	float *_hdrData, _exposure, _gamma;
	uint8_t *_ldrData;
	bool _multiThread;
	void _updateView();
};

#endif /* _TONEMAPPING_DIALOG_H_ */