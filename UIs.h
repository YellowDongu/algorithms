#pragma once
#include <vector>
#include <algorithm>


// Panel layer 정의, Panel 안에 있는 세부 ui들은 세부 레이어가 또 있다.
enum class LayerUI
{
	ActiveWIndow,// 화면 최상단에 있는 창들
	BackgroundedWindow,// 화면 최상단에 있는 창들에게 깔린 창들
	Base,// 기본 구성 ui들(팝업에 무조건 깔리는 애들) -> 창 선택 자체가 안됨
	inActive,// 꺼진 창들이나 꺼진 버튼등 화면에 안나타나는 애들
	END
};

class UI
{
public:
	UI() {}
	virtual ~UI() {}


	virtual void init() abstract;
	virtual void render() abstract;
	virtual void update() {}
	virtual void release() {}
	virtual bool checkCollide(int mouseX, int mouseY) abstract;

	void setActive(bool _active) { active = _active; }
	bool isActive() const { return active; }
	int layer() const { return DetailLayer; }

protected:
	int DetailLayer; // 작을수록 우선적으로 렌더
	bool active;
};

class Panel
{
public:
	Panel() {}
	virtual ~Panel() {}

	virtual void init() abstract;
	virtual void renderWindow() abstract;
	virtual void update() {}
	virtual void release() {}


	void render();
	void append(UI* _ui) { container.push_back(_ui); }
	void setActive(bool _active) { active = _active; }
	bool isActive() const { return active; }
	bool checkCollide(int mouseX, int mouseY);
	LayerUI layer() const { return baseLayer; }
protected:
	LayerUI baseLayer;
	bool active;
	int x, y; // 창 시작 좌표 -> vector2로 바꿀 수 있으면 바꾸셈
	int width, height;

	std::vector<UI*> container;

	bool sortByLayer(UI* a, UI* b) { return a->layer() > b->layer(); }
};


void Panel::render()
{
	if (baseLayer != LayerUI::Base)
	{
		renderWindow();
	}
	std::sort(container.begin(), container.end(), sortByLayer);
	for (UI* ui : container)
	{
		if (ui->isActive())
		{
			ui->render();
		}
	}

}


bool Panel::checkCollide(int mouseX, int mouseY)
{
	for (UI* ui : container)
	{
		if (ui->checkCollide(mouseX, mouseY))
		{
			return true;
		}
	}

	if ((x < mouseX || x + width > mouseX) && (y < mouseY || y + height > mouseY))
	{
		if (baseLayer != LayerUI::Base)
		{
			return true;
		}
	}
}




// 이건 씬 하나당 하나씩 만들까
class Canvas
{
public:
	Canvas();
	~Canvas();

	void render();

	void append(Panel* _ui) { container.push_back(_ui); }
	bool checkTriggered(int mouseX, int mouseY);

private:
	std::vector<Panel*> container;


	bool sortByLayer(Panel* a, Panel* b) { return (int)(a->layer()) > (int)(b->layer()); }
};

Canvas::Canvas()
{
}

Canvas::~Canvas()
{
}

void Canvas::render()
{
	std::sort(container.begin(), container.end(), sortByLayer);
	for (Panel* panel : container)
	{
		if (panel->isActive())
		{
			panel->render();
		}
	}
	/*
	for (int i = 0; i <= (int)LayerUI::END; i++)
	{
		LayerUI currentRender = (LayerUI)((int)LayerUI::END - i);
		for (Panel* panel : container)
		{
			if (panel->isActive() || currentRender == panel->layer())
			{
				panel->render();
			}
		}
	}
	*/
}

bool Canvas::checkTriggered(int mouseX, int mouseY)
{
	for (Panel* ui : container)
	{
		if (ui->checkCollide(mouseX, mouseY))
		{
			return true;
		}
	}
	return false;
}
