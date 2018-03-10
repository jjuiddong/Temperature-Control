//
// 2018-02-25, jjuiddong
// Parameters View
//
#pragma once


class cParametersView : public framework::cDockWindow
{
public:
	cParametersView();
	virtual ~cParametersView();

	bool Init(graphic::cRenderer &renderer);
	virtual void OnRender(const float deltaSeconds) override;


protected:
	void UpdateNTCRegistance();
	void UpdateTemperatureChanges();
	void UpdateNCTTemperature();
	void UpdateExternalTemperature();


public:
	// input voltage
	float m_incT;
	bool m_isOutVoltage;
};
