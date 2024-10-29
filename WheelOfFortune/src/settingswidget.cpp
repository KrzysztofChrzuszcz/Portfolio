#include "settingswidget.h"
#include <QDebug>

SettingsWidget::SettingsWidget(Settings& settings, QWidget* parent) :
    QDialog(parent),
    m_Settings(settings),
    m_Ui(new Ui::Settings)
{
    m_Ui->setupUi(this);
    connectControllers();
    m_Initialized = false;
    setInitValues();
    displayValues();
}

SettingsWidget::~SettingsWidget()
{
    disconnectControllers();
    delete m_Ui;
    qDebug() << "Destroyed";
}

void SettingsWidget::connectControllers()
{
    if (m_Ui->autostart_checkBox)
        connect(m_Ui->autostart_checkBox, SIGNAL(stateChanged(int)), this, SLOT(setAutoStart(int)));
    if (m_Ui->autoAdjust_checkBox)
        connect(m_Ui->autoAdjust_checkBox, SIGNAL(stateChanged(int)), this, SLOT(setAutoAdjust(int)));
    if (m_Ui->randMethod_comboBox)
        connect(m_Ui->randMethod_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setRandMethod(int)));
    if (m_Ui->refreshFrequency_comboBox)
        connect(m_Ui->refreshFrequency_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setRefreshFrequency(int)));
    if (m_Ui->minBrightness_slider)
        connect(m_Ui->minBrightness_slider, SIGNAL(valueChanged(int)), this, SLOT(setMinBrightness(int)));
    if (m_Ui->maxTime_slider)
        connect(m_Ui->maxTime_slider, SIGNAL(valueChanged(int)), this, SLOT(setMaxTime(int)));
    if (m_Ui->minPieAngle_slider)
        connect(m_Ui->minPieAngle_slider, SIGNAL(valueChanged(int)), this, SLOT(setMinPieAngle(int)));
    if (m_Ui->maxPieAngle_slider)
        connect(m_Ui->maxPieAngle_slider, SIGNAL(valueChanged(int)), this, SLOT(setMaxPieAngle(int)));
    if (m_Ui->randMin_slider)
        connect(m_Ui->randMin_slider, SIGNAL(valueChanged(int)), this, SLOT(setRandMin(int)));
    if (m_Ui->randMax_slider)
        connect(m_Ui->randMax_slider, SIGNAL(valueChanged(int)), this, SLOT(setRandMax(int)));
    if (m_Ui->quitButton)
        connect(m_Ui->quitButton, &QPushButton::clicked, this, &QWidget::close);
}

void SettingsWidget::disconnectControllers()
{
    disconnect();
}

void SettingsWidget::setInitValues()
{
    if (m_Ui->autostart_checkBox)
        m_Ui->autostart_checkBox->setChecked(m_Settings.m_AutoStart);

    if (m_Ui->autoAdjust_checkBox)
        m_Ui->autoAdjust_checkBox->setChecked(m_Settings.m_AutoAdjust);

    if (m_Ui->randMethod_comboBox)
        m_Ui->randMethod_comboBox->setCurrentIndex((int)m_Settings.m_RandomMathod); // TODO: Get back to it when Rand Methods will be ready. Use dynamic initialization.

    if (m_Ui->refreshFrequency_comboBox)
    {       
        m_Ui->refreshFrequency_comboBox->clear();
        for (int screenRefreshFrequency : m_Settings.m_ScreenRefreshFrequencies)
            m_Ui->refreshFrequency_comboBox->addItem(QString::number(screenRefreshFrequency));

        m_Ui->refreshFrequency_comboBox->setCurrentIndex(m_Settings.m_ScreenRefreshFrequencyIndex);
    }

    if (m_Ui->minBrightness_slider)
        m_Ui->minBrightness_slider->setValue(m_Settings.m_MinColorBrightness * 100);

    if (m_Ui->minPieAngle_slider)
        m_Ui->minPieAngle_slider->setValue(m_Settings.m_MinAngle);

    if (m_Ui->maxPieAngle_slider)
        m_Ui->maxPieAngle_slider->setValue(m_Settings.m_MaxAngle);

    if (m_Ui->maxTime_slider)
        m_Ui->maxTime_slider->setValue(m_Settings.m_MaxDurationTime);

    if (m_Ui->randMin_slider)
        m_Ui->randMin_slider->setValue(m_Settings.m_MinRandRange);

    if (m_Ui->randMax_slider)
        m_Ui->randMax_slider->setValue(m_Settings.m_MaxRandRange);

    m_Initialized = true;
}

void SettingsWidget::displayValues()
{
    ReadLock rLock(m_Settings.m_Lock);

    if (m_Ui->minBrightness_display)
        m_Ui->minBrightness_display->display(m_Settings.m_MinColorBrightness);

    if (m_Ui->maxTime_display)
        m_Ui->maxTime_display->display(m_Settings.m_MaxDurationTime);

    if (m_Ui->minPieAngle_display)
        m_Ui->minPieAngle_display->display(m_Settings.m_MinAngle);

    if (m_Ui->maxPieAngle_display)
        m_Ui->maxPieAngle_display->display(m_Settings.m_MaxAngle);

    if (m_Ui->randMin_display)
        m_Ui->randMin_display->display(m_Settings.m_MinRandRange);

    if (m_Ui->randMax_display)
        m_Ui->randMax_display->display(m_Settings.m_MaxRandRange);
}

void SettingsWidget::setAutoStart(int state)
{
    WriteLock wLock(m_Settings.m_Lock);
    if (m_Ui->autostart_checkBox)
        m_Settings.m_AutoStart = m_Ui->autostart_checkBox->isChecked();
}

void SettingsWidget::setAutoAdjust(int state)
{
    WriteLock wLock(m_Settings.m_Lock);
    if (m_Ui->autoAdjust_checkBox)
        m_Settings.m_AutoAdjust = m_Ui->autoAdjust_checkBox->isChecked();
}

void SettingsWidget::setRandMethod(int index)
{
    WriteLock wLock(m_Settings.m_Lock);
    m_Settings.m_RandomMathod = RandMethod(index);
}

void SettingsWidget::setRefreshFrequency(int index)
{
    WriteLock wLock(m_Settings.m_Lock);
    if (m_Ui->refreshFrequency_comboBox)
        if (m_Initialized)
            m_Settings.m_ScreenRefreshFrequencyIndex = index; // m_Ui->refreshFrequency_comboBox->currentIndex();
}

void SettingsWidget::setMinBrightness(int value)
{
    WriteLock wLock(m_Settings.m_Lock);
    if (m_Ui->minBrightness_slider)
    {
        m_Settings.m_MinColorBrightness = m_Ui->minBrightness_slider->value() / 100.f;
        if (m_Ui->minBrightness_display)
            m_Ui->minBrightness_display->display(m_Ui->minBrightness_slider->value() / 100.0);
    }
}

void SettingsWidget::setMaxTime(int value)
{
    WriteLock wLock(m_Settings.m_Lock);
    if (m_Ui->maxTime_slider)
    {
        m_Settings.m_MaxDurationTime = m_Ui->maxTime_slider->value();
        if (m_Ui->maxTime_display)
            m_Ui->maxTime_display->display(m_Settings.m_MaxDurationTime);
    }
}

void SettingsWidget::setMinPieAngle(int value)
{
    WriteLock wLock(m_Settings.m_Lock);
    if (m_Ui->minPieAngle_slider)
    {
        m_Settings.m_MinAngle = m_Ui->minPieAngle_slider->value();
        if (m_Ui->minPieAngle_display)
            m_Ui->minPieAngle_display->display(m_Settings.m_MinAngle);

        if (m_Ui->maxPieAngle_slider)
        {
            m_Ui->maxPieAngle_slider->setMinimum(m_Settings.m_MinAngle);
            m_Ui->maxPieAngle_minimumLabel->setText(QString::number(m_Settings.m_MinAngle));
        }

        m_Settings.m_MaxPositionsAmount = 360.f / m_Settings.m_MinAngle;
    }
}

void SettingsWidget::setMaxPieAngle(int value)
{
    WriteLock wLock(m_Settings.m_Lock);
    if (m_Ui->maxPieAngle_slider)
    {
        m_Settings.m_MaxAngle = m_Ui->maxPieAngle_slider->value();
        if (m_Ui->maxPieAngle_display)
            m_Ui->maxPieAngle_display->display(m_Settings.m_MaxAngle);
    }
}

void SettingsWidget::setRandMin(int value)
{
    WriteLock wLock(m_Settings.m_Lock);
    if (m_Ui->randMin_slider)
    {
        m_Settings.m_MinRandRange = m_Ui->randMin_slider->value();
        if (m_Ui->randMin_display)
            m_Ui->randMin_display->display(m_Settings.m_MinRandRange);
    }
}

void SettingsWidget::setRandMax(int value)
{
    WriteLock wLock(m_Settings.m_Lock);
    if (m_Ui->randMax_slider)
    {
        m_Settings.m_MaxRandRange = m_Ui->randMax_slider->value();
        if (m_Ui->randMax_display)
            m_Ui->randMax_display->display(m_Settings.m_MaxRandRange);
    }
}
