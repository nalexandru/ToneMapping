#include <QtGui/qcheckbox.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qboxlayout.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qmessagebox.h>

#include <dzapp.h>
#include <dzfilefilter.h>
#include <dzmainwindow.h>
#include <dzfloatslider.h>

#define TINYEXR_IMPLEMENTATION
#include "tinyexr.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "Tonemapping.h"
#include "TonemappingDialog.h"

ToneMappingDialog::ToneMappingDialog(QWidget *parent)
	: DzBasicDialog(parent, "Tone Mapping"),
	_imgWidth(0), _imgHeight(0), _hdrData(nullptr), _ldrData(nullptr),
	_exposure(0.0001f), _gamma(2.2f), _multiThread(true), _mode(TM_ACES)
{
	_imageView = new QLabel(this);

	DzFloatSlider *exposureSlider = new DzFloatSlider(this, "Exposure");
	DzFloatSlider *gammaSlider = new DzFloatSlider(this, "Gamma");
	QPushButton *loadButton = new QPushButton("Load Image", this);
	QPushButton *saveButton = new QPushButton("Save Image", this);
	QCheckBox *mtCheckBox = new QCheckBox("Multithreading", this);
	QComboBox *modeComboBox = new QComboBox(this);

	exposureSlider->setValue(1.f);
	gammaSlider->setValue(2.2f);
	mtCheckBox->setChecked(true);

	modeComboBox->addItem("ACES");
	modeComboBox->addItem("Uncharted 2");
	modeComboBox->addItem("Hejl-Richard");
	modeComboBox->addItem("Reinhard");
	modeComboBox->setCurrentIndex(0);

	modeComboBox->setGeometry(10, 10, 75, 25);
	exposureSlider->setGeometry(10, 40, 200, 25);
	gammaSlider->setGeometry(10, 70, 200, 25);
	loadButton->setGeometry(10, 100, 75, 25);
	saveButton->setGeometry(90, 100, 75, 25);
	mtCheckBox->setGeometry(10, 130, 75, 25);

	_imageView->setGeometry(215, 10, 1280, 720);

	this->setMinimumWidth(1505);
	this->setMinimumHeight(740);
	this->setFixedSize(1505, 740);

	this->showAcceptButton(false);
	this->showCancelButton(false);
	this->showHelpButton(false);

	this->setWindowTitle("Tone Mapping");

	connect(loadButton, SIGNAL(clicked()), this, SLOT(loadImage()));
	connect(saveButton, SIGNAL(clicked()), this, SLOT(saveImage()));
	connect(mtCheckBox, SIGNAL(stateChanged(int)), this, SLOT(mtStateChanged(int)));
	connect(gammaSlider, SIGNAL(valueChanged(float)), this, SLOT(gammaChanged(float)));
	connect(modeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(modeChanged(int)));
	connect(exposureSlider, SIGNAL(valueChanged(float)), this, SLOT(exposureChanged(float)));
}

void
ToneMappingDialog::loadImage()
{
	DzMainWindow *wnd = dzApp->getInterface();
	if (!wnd)
		return;

	QString file;

	DzFileFilter filter("OpenEXR Bitmap", "exr");

	if (!wnd->doFileDialog(true, "Open Canvas", "", filter, file, NULL, this))
		return;

	free(_ldrData);
	delete _hdrData;

	const char *err = NULL;
	if (LoadEXR(&_hdrData, &_imgWidth, &_imgHeight, file.toStdString().c_str(), &err) < 0) {
		QMessageBox::warning(this, tr("Error"), tr("Failed to open file %1").arg(err), QMessageBox::Ok);
		return;
	}

	_ldrData = (uint8_t *)calloc((size_t)_imgWidth * (size_t)_imgHeight * 3L, sizeof(*_ldrData));

	_updateView();
}

void
ToneMappingDialog::saveImage()
{
	DzMainWindow *wnd = dzApp->getInterface();
	if (!wnd)
		return;

	int ext;
	QString file;

	DzFileFilter filter;

	filter.newFilter("Truevision TARGA");
	filter.addExtension("tga");

	filter.newFilter("Portable Network Graphics");
	filter.addExtension("png");

	filter.newFilter("Bitmap");
	filter.addExtension("bmp");

	filter.newFilter("JPEG");
	filter.addExtension("jpg");
	
	if (!wnd->doFileDialog(false, "Save Image", "", filter, file, &ext, this))
		return;

	uint8_t *imgData = (uint8_t *)calloc((size_t)_imgWidth * (size_t)_imgHeight * 4L, sizeof(*imgData));
	if (!imgData) {
		QMessageBox::warning(this, tr("Error"), tr("Failed to allocate memory"), QMessageBox::Ok);
		return;
	}

	uint8_t *ldr = _ldrData;
	for (uint32_t i = 0; i < _imgWidth * _imgHeight * 4; i += 4) {
		imgData[i    ] = *ldr++;
		imgData[i + 1] = *ldr++;
		imgData[i + 2] = *ldr++;
		imgData[i + 3] = _hdrData[i + 3] * 255.f;
	}

	int rc;
	switch (ext) {
	case 1: rc = stbi_write_png(file.toStdString().c_str(), _imgWidth, _imgHeight, 4, imgData, 0); break;
	case 2: rc = stbi_write_bmp(file.toStdString().c_str(), _imgWidth, _imgHeight, 4, imgData); break;
	case 3: rc = stbi_write_jpg(file.toStdString().c_str(), _imgWidth, _imgHeight, 4, imgData, 100); break;
	default:
	case 0: rc = stbi_write_tga(file.toStdString().c_str(), _imgWidth, _imgHeight, 4, imgData); break;
	}

	free(imgData);

	if (!rc) {
		QMessageBox::warning(this, tr("Error"), tr("Failed to write file"), QMessageBox::Ok);
	}
}

void
ToneMappingDialog::exposureChanged(float val)
{
	if (!_hdrData || !_ldrData)
		return;

	_exposure = val * 0.0001f;

	_updateView();
}

void
ToneMappingDialog::gammaChanged(float val)
{
	if (!_hdrData || !_ldrData)
		return;

	_gamma = val;

	_updateView();
}

void
ToneMappingDialog::mtStateChanged(int state)
{
	_multiThread = state == Qt::Checked;
}

void
ToneMappingDialog::modeChanged(int mode)
{
	_mode = (enum ToneMapMode)mode;
	_updateView();
}

void
ToneMappingDialog::_updateView()
{
	if (_multiThread)
		toneMapMT(_mode, _hdrData, _ldrData, _imgWidth, _imgHeight, _exposure, _gamma);
	else
		toneMap(_mode, _hdrData, _ldrData, _imgWidth, _imgHeight, _exposure, _gamma);

	QImage image((uchar *)_ldrData, _imgWidth, _imgHeight, QImage::Format::Format_RGB888);
	_imageView->setPixmap(QPixmap::fromImage(image).scaled(1280, 720, Qt::AspectRatioMode::KeepAspectRatio));
}

ToneMappingDialog::~ToneMappingDialog()
{
	free(_ldrData);
	delete _hdrData;
}