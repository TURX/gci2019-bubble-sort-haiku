/* 
 * The MIT License (MIT)
 * 
 * Copyright 2019, Ruixuan Tu
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "BubbleSort.h"

#include <math.h>
#include <stdlib.h>

#include <Alignment.h>
#include <Button.h>
#include <LayoutBuilder.h>
#include <Slider.h>
#include <String.h>
#include <StringView.h>
#include <Window.h>

#define ARRAY_SIZE(a) \
	(sizeof(a) / sizeof(a[0]))

static const BString kName = "Bubble Sort";
static const BString kAuthor = "Ruixuan Tu";
static const rgb_color kBlackColor = { 0, 0, 0, 255 };
static const rgb_color kWhiteColor = { 255, 255, 255, 255 };


extern "C" BScreenSaver*
instantiate_screen_saver(BMessage* msg, image_id id)
{
	return new BubbleSort(msg, id);
}


BubbleSort::BubbleSort(BMessage* archive, image_id id)
	:
	BScreenSaver(archive, id)
{
}


BubbleSort::~BubbleSort()
{
}


void BubbleSort::StartConfig(BView* view)
{
	BWindow* win = view->Window();
	if (win)
		win->AddHandler(this);

	BStringView* v1 = new BStringView("name", "Bubble Sort");
	v1->SetFont(be_bold_font);
	BStringView* v2 = new BStringView("author", "by Ruixuan Tu");

	BLayoutBuilder::Group<>(view, B_VERTICAL, B_USE_ITEM_SPACING)
		.SetInsets(B_USE_WINDOW_INSETS)
		.SetExplicitAlignment(BAlignment(B_ALIGN_HORIZONTAL_CENTER, B_ALIGN_TOP))
		.AddGroup(B_HORIZONTAL)
			.Add(v1)
			.Add(v2)
			.AddGlue()
		.End();
}


status_t BubbleSort::StartSaver(BView* view, bool prev)
{
	srandom(time(NULL));

	BRect rect = view->Bounds();
	fWidth = rect.IntegerWidth();
	fHeight = rect.IntegerHeight();
	int arr[fWidth / 5];
	fArr = arr;

	view->SetLineMode(B_ROUND_CAP, B_ROUND_JOIN);
	view->SetFlags(view->Flags() | B_SUBPIXEL_PRECISE);

	view->SetLowColor(kBlackColor);
	view->SetViewColor(kBlackColor);
	view->SetHighColor(kWhiteColor);

	view->SetPenSize(5);

	_Restart(view);

	SetTickSize(50000); // 0.05 sec

	return B_OK;
}


void BubbleSort::GenerateArray()
{
	for (int i = 0; i < fWidth / 5; i++)
		fArr[i] = abs(random()) % fHeight;
}


void BubbleSort::_Restart(BView* view)
{
	fNeedsRestart = false;

	view->FillRect(view->Bounds(), B_SOLID_LOW);

	GenerateArray();

	for (int i = 0; i <= fWidth / 5; i++)
		view->StrokeLine(BPoint(i * 5, fHeight - fArr[i]), BPoint(i * 5, fHeight), B_SOLID_HIGH);

	fI = 0;
	fL = fWidth / 5;
}


void BubbleSort::Draw(BView* view, int32 frame)
{
	if (fNeedsRestart)
		_Restart(view);

	if (fI >= fL) {
		fNeedsRestart = true;
		return;
	} else
		fI++;

	if (fArr[fI] > fArr[fI + 1]) {
		int tmp = fArr[fI];
		fArr[fI] = fArr[fI + 1];
		fArr[fI + 1] = tmp;
		view->BeginLineArray(2);
		view->AddLine(BPoint(fI * 5, 0), BPoint(fI * 5, fHeight), kBlackColor);
		view->AddLine(BPoint((fI + 1) * 5, 0), BPoint((fI + 1) * 5, fHeight), kBlackColor);
		view->EndLineArray();
		view->StrokeLine(BPoint(fI * 5, fHeight - fArr[fI]), BPoint(fI * 5, fHeight), B_SOLID_HIGH);
		view->StrokeLine(BPoint((fI + 1) * 5, fHeight - fArr[fI + 1]), BPoint((fI + 1) * 5, fHeight), B_SOLID_HIGH);
	}

	if (fI == fL - 1) {
		fI = -1;
		fL--;
	}
}

