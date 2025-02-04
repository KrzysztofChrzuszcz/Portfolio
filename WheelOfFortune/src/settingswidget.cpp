#include "settingswidget.h"
#include "mainwindow.h"

#include <QDebug>

SettingsWidget::SettingsWidget(Settings& settings, QWidget* parent) :
    QDialog(parent),
    m_Settings(settings),
    m_Ui(new Ui::Settings)
{
    m_Ui->setupUi(this);
    m_Initialized = false;
    initRangeSliders();
    setInitValues();
    connectControllers();
    displayValues();
}

SettingsWidget::~SettingsWidget()
{
    emit destroyed(this); // signal should be emitted manually OR disconnectControllers method should disconnect all signals one by one
    disconnectControllers();
    delete m_Ui;
}

void SettingsWidget::initRangeSliders()
{
    if (m_Ui->PieAngleRange_slider)
    {
        m_Ui->PieAngleRange_slider->SetType(RangeSlider::Option::DoubleHandles);

        m_Ui->PieAngleRange_slider->SetRange(15, 60);
        // Issue with used class is fixed by calling SetMinimum explicitly
        m_Ui->PieAngleRange_slider->SetMinimum(15);
        m_Ui->PieAngleRange_slider->SetMaximum(60);
    }

    if (m_Ui->randRange_slider)
    {
        m_Ui->randRange_slider->SetType(RangeSlider::Option::DoubleHandles);

        m_Ui->randRange_slider->SetRange(2000, 5000);
        // Issue with used class is fixed by calling SetMinimum explicitly
        m_Ui->randRange_slider->SetMinimum(2000);
        m_Ui->randRange_slider->SetMaximum(5000);
    }
}

void SettingsWidget::connectControllers()
{
    if (m_Ui->autostart_checkBox)
        connect(m_Ui->autostart_checkBox, SIGNAL(stateChanged(int)), this, SLOT(setAutoStart(int)));
    if (m_Ui->autoAdjust_checkBox)
        connect(m_Ui->autoAdjust_checkBox, SIGNAL(stateChanged(int)), this, SLOT(setAutoAdjust(int)));
    if (m_Ui->randGenerator_comboBox)
        connect(m_Ui->randGenerator_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setRandGenerator(int)));
    if (m_Ui->refreshFrequency_comboBox)
        connect(m_Ui->refreshFrequency_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setRefreshFrequency(int)));
    if (m_Ui->minBrightness_slider)
        connect(m_Ui->minBrightness_slider, SIGNAL(valueChanged(int)), this, SLOT(setMinBrightness(int)));
    if (m_Ui->maxTime_slider)
        connect(m_Ui->maxTime_slider, SIGNAL(valueChanged(int)), this, SLOT(setMaxTime(int)));
    if (m_Ui->PieAngleRange_slider)
        connect(m_Ui->PieAngleRange_slider, SIGNAL(lowerValueChanged(int)), this, SLOT(setMinPieAngle(int)));
    if (m_Ui->PieAngleRange_slider)
        connect(m_Ui->PieAngleRange_slider, SIGNAL(upperValueChanged(int)), this, SLOT(setMaxPieAngle(int)));
    if (m_Ui->randRange_slider)
        connect(m_Ui->randRange_slider, SIGNAL(lowerValueChanged(int)), this, SLOT(setRandMin(int)));
    if (m_Ui->randRange_slider)
        connect(m_Ui->randRange_slider, SIGNAL(upperValueChanged(int)), this, SLOT(setRandMax(int)));
    if (m_Ui->quitButton)
        connect(m_Ui->quitButton, &QPushButton::clicked, this, &QWidget::close);
}

void SettingsWidget::disconnectControllers()
{
    disconnect(); /// This may cause issue with QObject::destroyed signal
}

void SettingsWidget::setInitValues()
{
    if (m_Ui->autostart_checkBox)
        m_Ui->autostart_checkBox->setChecked(m_Settings.m_AutoStart);

    if (m_Ui->autoAdjust_checkBox)
        m_Ui->autoAdjust_checkBox->setChecked(m_Settings.m_AutoAdjust);

    if (m_Ui->randGenerator_comboBox)
        m_Ui->randGenerator_comboBox->setCurrentIndex(static_cast<int>(m_Settings.m_RandomGenerator));

    if (m_Ui->refreshFrequency_comboBox)
    {       
        m_Ui->refreshFrequency_comboBox->clear();
        for (int screenRefreshFrequency : m_Settings.m_ScreenRefreshFrequencies)
            m_Ui->refreshFrequency_comboBox->addItem(QString::number(screenRefreshFrequency));

        m_Ui->refreshFrequency_comboBox->setCurrentIndex(m_Settings.m_ScreenRefreshFrequencyIndex);
    }

    if (m_Ui->minBrightness_slider)
        m_Ui->minBrightness_slider->setValue(m_Settings.m_MinColorBrightness * 100);

    if (m_Ui->PieAngleRange_slider)
    {
        m_Ui->PieAngleRange_slider->SetLowerValue(m_Settings.m_MinAngle);
        m_Ui->PieAngleRange_slider->SetUpperValue(m_Settings.m_MaxAngle);
    }

    if (m_Ui->maxTime_slider)
        m_Ui->maxTime_slider->setValue(m_Settings.m_MaxDurationTime);

    if (m_Ui->randRange_slider)
    {
        m_Ui->randRange_slider->SetLowerValue(m_Settings.m_MinRandRange);
        m_Ui->randRange_slider->SetUpperValue(m_Settings.m_MaxRandRange);
    }

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
    {
        m_Settings.m_AutoStart = m_Ui->autostart_checkBox->isChecked();
        m_Settings.updateLastChangeTime();
    }
}

void SettingsWidget::setAutoAdjust(int state)
{
    WriteLock wLock(m_Settings.m_Lock);
    if (m_Ui->autoAdjust_checkBox)
    {
        m_Settings.m_AutoAdjust = m_Ui->autoAdjust_checkBox->isChecked();
        m_Settings.updateLastChangeTime();
    }
}

void SettingsWidget::setRandGenerator(int index)
{
    WriteLock wLock(m_Settings.m_Lock);
    m_Settings.m_RandomGenerator = RandGenerator(index);
    m_Settings.updateLastChangeTime();
}

void SettingsWidget::setRefreshFrequency(int index)
{
    WriteLock wLock(m_Settings.m_Lock);
    if (m_Ui->refreshFrequency_comboBox)
        if (m_Initialized)
        {
            m_Settings.m_ScreenRefreshFrequencyIndex = index; // m_Ui->refreshFrequency_comboBox->currentIndex();
            m_Settings.updateLastChangeTime();
        }
}

void SettingsWidget::setMinBrightness(int value)
{
    WriteLock wLock(m_Settings.m_Lock);
    if (m_Ui->minBrightness_slider)
    {
        m_Settings.m_MinColorBrightness = m_Ui->minBrightness_slider->value() / 100.f;
        m_Settings.updateLastChangeTime();
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
        m_Settings.updateLastChangeTime();
        if (m_Ui->maxTime_display)
            m_Ui->maxTime_display->display(m_Settings.m_MaxDurationTime);
    }
}

void SettingsWidget::setMinPieAngle(int value)
{
    WriteLock wLock(m_Settings.m_Lock);
    if (m_Ui->PieAngleRange_slider)
    {
        m_Settings.m_MinAngle = m_Ui->PieAngleRange_slider->GetLowerValue();
        m_Settings.updateLastChangeTime();
        if (m_Ui->minPieAngle_display)
            m_Ui->minPieAngle_display->display(m_Settings.m_MinAngle);
        float max = 360.f / (float)m_Settings.m_MinAngle;
    }
}

void SettingsWidget::setMaxPieAngle(int value)
{
    WriteLock wLock(m_Settings.m_Lock);
    if (m_Ui->PieAngleRange_slider)
    {
        m_Settings.m_MaxAngle = m_Ui->PieAngleRange_slider->GetUpperValue();
        m_Settings.updateLastChangeTime();
        if (m_Ui->maxPieAngle_display)
            m_Ui->maxPieAngle_display->display(m_Settings.m_MaxAngle);
    }
}

void SettingsWidget::setRandMin(int value)
{
    WriteLock wLock(m_Settings.m_Lock);
    if (m_Ui->randRange_slider)
    {
        m_Settings.m_MinRandRange = m_Ui->randRange_slider->GetLowerValue();
        m_Settings.updateLastChangeTime();
        if (m_Ui->randMin_display)
            m_Ui->randMin_display->display(m_Settings.m_MinRandRange);
    }
}

void SettingsWidget::setRandMax(int value)
{
    WriteLock wLock(m_Settings.m_Lock);
    if (m_Ui->randRange_slider)
    {
        m_Settings.m_MaxRandRange = m_Ui->randRange_slider->GetUpperValue();
        m_Settings.updateLastChangeTime();
        if (m_Ui->randMax_display)
            m_Ui->randMax_display->display(m_Settings.m_MaxRandRange);
    }
}
