//
// 2018-02-28, jjuiddong
// Output View
//
#pragma once


class cOutputView : public framework::cDockWindow
{
public:
	cOutputView();
	virtual ~cOutputView();

	bool Init(graphic::cRenderer &renderer);
	virtual void OnRender(const float deltaSeconds) override;


public:
	float m_incT;
	int m_calcCnt;
	int m_riseCnt;
	float m_incCurrent;
	float m_calcCurrent;
	float m_totalCurrent;
};
