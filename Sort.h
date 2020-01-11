#include <ScreenSaver.h>

class BSlider;
class BMenuField;
class BMenuItem;

class Sort : public BScreenSaver, public BHandler {
public:
						Sort(BMessage* archive, image_id id);
						~Sort();

	virtual void		StartConfig(BView* view);
	virtual	status_t	SaveState(BMessage* into) const;
	virtual	void		MessageReceived(BMessage* message);

	virtual status_t	StartSaver(BView* view, bool preview);
	virtual void		Draw(BView* view, int32 frame);

private:
	void		_Restart(BView* view);
	void		Swap(int& i, int& j);
	void		GenerateArray();

	void		BubbleSort(BView* view, int32 frame);
	void		InsertionSort(BView* view, int32 frame);

	int			fWidth;
	int			fHeight;

	int*		fArr;

	int			fI;
	int			fL;
	int			fKey;

	bool		fNeedsRestart;
	bool		fNeedsDraw;

	BSlider*	fSpeedS;
	int			fSpeed;

	BMenuField*	fTypeMenuField;
	int			fType;
};

