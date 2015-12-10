#pragma once

class Game {
public:
	void Play();
	virtual void Load();
	virtual void Update();
	virtual void Render();
	virtual void Unload();
	void End();

protected:
	double msPerUpdate_ = 1000. / 20.;
	double msPerRender_ = 1000. / 60.;
	double gameSpeedMultiplier_ = 1;

private:
	bool gameOver_ = false;
};