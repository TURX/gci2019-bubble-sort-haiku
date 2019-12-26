#include <Handler.h>
#include <ScreenSaver.h>
#include <ObjectList.h>

class BubbleSort : public BScreenSaver, public BHandler {
public:
						BubbleSort(BMessage* archive, image_id id);
						~BubbleSort();

	virtual void		StartConfig(BView* view);

	virtual status_t	StartSaver(BView* view, bool preview);
	virtual void		Draw(BView* view, int32 frame);

private:
	void	_Restart(BView* view);
	void	Swap(int& i, int& j);
	void	GenerateArray();

	int		fWidth;
	int		fHeight;

	int*	fArr;

	int		fI;
	int		fL;

	bool	fNeedsRestart;
};

