/* 
 * The MIT License (MIT)
 * 
 * Copyright 2019-2020, Ruixuan Tu
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

#include "Sort.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include <InterfaceKit.h>
#include <MenuField.h>
#include <PopUpMenu.h>
#include <Slider.h>
#include <String.h>
#include <TextView.h>
#include <Window.h>

#define ARRAY_SIZE(a) \
	(sizeof(a) / sizeof(a[0]))

static const BString kName = "Sort ScreenSaver";
static const BString kAuthor = "Ruixuan Tu";
static const rgb_color kBlackColor = { 0, 0, 0, 255 };
static const rgb_color kWhiteColor = { 255, 255, 255, 255 };
static const int kPenSize = 5;
static const uint32 kMsgSetSpeed = 'stsp';
static const uint32 kMsgSetType = 'sttp';

enum {
	TYPE_BUBBLE = 1,
	TYPE_INSERTION = 2
};


extern "C" BScreenSaver*
instantiate_screen_saver(BMessage* msg, image_id id)
{
	return new Sort(msg, id);
}


Sort::Sort(BMessage* archive, image_id id)
	:
	BScreenSaver(archive, id),
	fSpeed(5),
	fType(TYPE_BUBBLE)
{
	if (archive) {
		if (archive->FindInt32("speed", &fSpeed) != B_OK)
			fSpeed = 5;
		if (archive->FindInt32("type", &fType) != B_OK)
			fType = TYPE_BUBBLE;
	}
}


Sort::~Sort()
{
}


void Sort::StartConfig(BView* view)
{
	BRect bounds = view->Bounds();
	bounds.InsetBy(10, 10);
	BRect frame(0, 0, bounds.Width(), 20);

	fSpeedS = new BSlider(frame, "speed setting",
		"Speed:", new BMessage(kMsgSetSpeed), 1, 10,
		B_BLOCK_THUMB, B_FOLLOW_LEFT_RIGHT | B_FOLLOW_BOTTOM);

	fSpeedS->SetValue(fSpeed);
	fSpeedS->SetHashMarks(B_HASH_MARKS_BOTTOM);
	fSpeedS->SetHashMarkCount(10);

	fSpeedS->ResizeToPreferred();
	bounds.bottom -= fSpeedS->Bounds().Height() * 1.5;
	fSpeedS->MoveTo(bounds.LeftBottom());

	view->AddChild(fSpeedS);

	BPopUpMenu* popUpMenu = new BPopUpMenu("");
	BString label;

	label.SetTo("Bubble Sort");
	BMessage* message1 = new BMessage(kMsgSetType);
	message1->AddInt32("type", TYPE_BUBBLE);
	const char* l1 = label.String();
	BMenuItem* item1 = new BMenuItem(l1, message1);
	popUpMenu->AddItem(item1);
	item1->SetMarked(fType == TYPE_BUBBLE);

	label.SetTo("Insertion Sort");
	BMessage* message2 = new BMessage(kMsgSetType);
	message2->AddInt32("type", TYPE_INSERTION);
	const char* l2 = label.String();
	BMenuItem* item2 = new BMenuItem(l2, message2);
	popUpMenu->AddItem(item2);
	item2->SetMarked(fType == TYPE_INSERTION);

	fTypeMenuField = new BMenuField("type setting", "Sort type:", popUpMenu);
	fTypeMenuField->ResizeToPreferred();
	bounds.bottom -= fTypeMenuField->Bounds().Height() * 1.5;
	fTypeMenuField->MoveTo(bounds.LeftBottom());

	view->AddChild(fTypeMenuField);

	BRect textRect = bounds;
	textRect.OffsetTo(0, 0);
	BTextView* textView = new BTextView(bounds, B_EMPTY_STRING, textRect,
		B_FOLLOW_ALL, B_WILL_DRAW);
	textView->SetViewColor(view->ViewColor());

	BString aboutScreenSaver("%screenSaverName%\n\n"
		B_UTF8_COPYRIGHT " 2019-2020 Ruixuan Tu <turx2003@gmail.com>");
	BString screenSaverName("Sort ScreenSaver");

	aboutScreenSaver.ReplaceFirst("%screenSaverName%", screenSaverName);
	textView->Insert(aboutScreenSaver);

	textView->SetStylable(true);
	textView->SetFontAndColor(0, screenSaverName.Length(), be_bold_font);

	textView->MakeEditable(false);

	view->AddChild(textView);

	if (BWindow* window = view->Window())
		window->AddHandler(this);

	fTypeMenuField->Menu()->SetTargetForItems(this);
	fSpeedS->SetTarget(this);
}


status_t Sort::SaveState(BMessage* into) const
{
	status_t status;

	if ((status = into->AddInt32("speed", fSpeed)) != B_OK)
		return status;
	
	if ((status = into->AddInt32("type", fType)) != B_OK)
		return status;

	return B_OK;
}


void Sort::MessageReceived(BMessage* message)
{
	switch (message->what) {
		case kMsgSetSpeed:
			int speed;
			if (message->FindInt32("be:value", &speed) == B_OK)
				fSpeed = speed;
			break;
		
		case kMsgSetType:
			int type;
			if (message->FindInt32("be:value", &type) == B_OK) {
				for (int i = 0; i < 1; i++) printf("msg set type info: %d %d\n", fType, type);
				fType = type;
			} else {
				for (int i = 0; i < 1; i++) printf("msg set type err\n");
			}
			break;

		default:
			BHandler::MessageReceived(message);
	}
}


status_t Sort::StartSaver(BView* view, bool prev)
{
	srandom(time(NULL));

	BRect rect = view->Bounds();
	fWidth = rect.IntegerWidth();
	fHeight = rect.IntegerHeight();
	int arr[fWidth / kPenSize];
	fArr = arr;

	view->SetLineMode(B_ROUND_CAP, B_ROUND_JOIN);

	view->SetLowColor(kBlackColor);
	view->SetViewColor(kBlackColor);
	view->SetHighColor(kWhiteColor);

	view->SetPenSize(kPenSize - 1);

	_Restart(view);

	SetTickSize(100000 / fSpeed); // default: 50000 - 0.05 sec

	return B_OK;
}


void Sort::GenerateArray()
{
	for (int i = 0; i < fWidth / kPenSize; i++)
		fArr[i] = abs(random()) % fHeight;
}


void Sort::_Restart(BView* view)
{
	fNeedsRestart = false;

	view->FillRect(view->Bounds(), B_SOLID_LOW);

	GenerateArray();

	switch (fType) {
		case TYPE_BUBBLE:
			fI = 0;
			fL = fWidth / kPenSize;
			break;

		case TYPE_INSERTION:
			fI = 1;
			fKey = fArr[fI];
			fL = fI - 1;
			break;
	}

	fNeedsDraw = true;
}


void Sort::Draw(BView* view, int32 frame)
{
	if (fNeedsRestart)
		_Restart(view);
	
	if (fNeedsDraw) {
		view->BeginLineArray(fWidth / kPenSize);
		for (int i = 0; i <= fWidth / kPenSize; i++)
			view->AddLine(BPoint(i * kPenSize, fHeight - fArr[i]), BPoint(i * kPenSize, fHeight), {255, 255, 255});
		view->EndLineArray();
		fNeedsDraw = false;
	}

	switch (fType) {
		case TYPE_BUBBLE:
			BubbleSort(view, frame);
			break;

		case TYPE_INSERTION:
			InsertionSort(view, frame);
			break;
	}
}


void Sort::BubbleSort(BView* view, int32 frame) {
	if (fI >= fL) {
		fNeedsRestart = true;
		return;
	} else
		fI++;

	if (fArr[fI] > fArr[fI + 1]) {
		int tmp = fArr[fI];
		fArr[fI] = fArr[fI + 1];
		fArr[fI + 1] = tmp;
		view->StrokeLine(BPoint(fI * kPenSize, 0), BPoint(fI * kPenSize, fHeight), B_SOLID_LOW);
		view->StrokeLine(BPoint(fI * kPenSize, fHeight - fArr[fI]), BPoint(fI * kPenSize, fHeight), B_SOLID_HIGH);
		view->StrokeLine(BPoint((fI + 1) * kPenSize, 0), BPoint((fI + 1) * kPenSize, fHeight), B_SOLID_LOW);
		view->StrokeLine(BPoint((fI + 1) * kPenSize, fHeight - fArr[fI + 1]), BPoint((fI + 1) * kPenSize, fHeight), B_SOLID_HIGH);
	}

	if (fI == fL - 1) {
		fI = -1;
		fL--;
	}
}


void Sort::InsertionSort(BView* view, int32 frame) {
	if (fI > fWidth / kPenSize) {
		fNeedsRestart = true;
		return;
	}

	if (fL >= 0 && fArr[fL] > fKey) {
		fArr[fL + 1] = fArr[fL];
		fL = fL - 1;
		view->StrokeLine(BPoint((fL + 1) * kPenSize, 0), BPoint((fL + 1) * kPenSize, fHeight), B_SOLID_LOW);
		view->StrokeLine(BPoint((fL + 1) * kPenSize, fHeight - fArr[fL + 1]), BPoint((fL + 1) * kPenSize, fHeight), B_SOLID_HIGH);
	} else {
		fArr[fL + 1] = fKey;
		view->StrokeLine(BPoint((fL + 1) * kPenSize, 0), BPoint((fL + 1) * kPenSize, fHeight), B_SOLID_LOW);
		view->StrokeLine(BPoint((fL + 1) * kPenSize, fHeight - fArr[fL + 1]), BPoint((fL + 1) * kPenSize, fHeight), B_SOLID_HIGH);
		fKey = fArr[fI];
		fL = fI - 1;
		fI++;
	}
}

