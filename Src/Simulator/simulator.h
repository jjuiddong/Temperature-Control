//
// 2018-02-24, jjuiddong
//
//
#pragma once


class cInputView;
class cOutputView;
class cParametersView;

class cViewer : public framework::cGameMain2
{
public:
	cViewer();
	virtual ~cViewer();

	virtual bool OnInit() override;
	virtual void OnUpdate(const float deltaSeconds) override;
	virtual void OnRender(const float deltaSeconds) override;
	virtual void OnEventProc(const sf::Event &evt) override;


public:
	cInputView *m_inputView;	
	cOutputView *m_outputView;
	cParametersView *m_paramView;
};
