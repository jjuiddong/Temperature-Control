//
// 2018-02-24, jjuiddong
// Input View
//
#pragma once


class cInputView : public framework::cDockWindow
{
public:
	cInputView();
	virtual ~cInputView();

	bool Init(graphic::cRenderer &renderer);
	virtual void OnRender(const float deltaSeconds) override;


public:
	// input voltage
	float m_incT;
	float m_timeSeconds; // seconds
	int m_ctrlPeak;
	bool m_isOutVoltage;
};
