#include <Handler.h>
#include <ScreenSaver.h>
#include <ObjectList.h>

class BubbleSort : public BScreenSaver, public BHandler {
public:
						BubbleSort(BMessage* archive, image_id id);
						~BubbleSort();

	virtual void		StartConfig(BView* view);

	virtual status_t	StartSaver(BView* view, bool preview);
	virtual void		Draw(BView* view, int32 change);

	virtual void		MessageReceived(BMessage* msg);
	virtual status_t	SaveState(BMessage* into) const;

private:
	void	_Restart(BView* view);
	void	Swap(int& i, int& j);
	void	Sort(BView* view);
	void	GenerateArray();

	int		fWidth;
	int		fHeight;

	int*	fArr;

	bool	fNeedsRestart;
};

