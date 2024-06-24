#pragma once
class Single
{
private:
	Single();
public:
	static Single* GetInst()
	{
		if (_inst == nullptr)
			_inst = new Single;
		return _inst;
	}
private:
	static Single* _inst;
};

