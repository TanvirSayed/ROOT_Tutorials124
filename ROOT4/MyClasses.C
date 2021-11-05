// ROOT library objects
#include <TObject.h> // to derive classes

#ifndef MyClasses
#define MyClasses

//________________________MyEvent class



//create a "MyEvent" object with MyEvent* event = new MyEvent();
class MyEvent : public TObject
{
	public:
		Int_t nTracks;
		Double_t fV2;
		MyEvent();
		MyEvent(Int_t NTracks, Double_t FV2);

		ClassDef(MyEvent, 1); // Help class
};

ClassImp(MyEvent) // No semi-colon!

MyEvent::MyEvent():
	TObject(),
		nTracks(-1), //set default values of data members to -1 
		fV2(-1) {}

MyEvent::MyEvent(Int_t NTracks, Double_t FV2): 
	TObject(),
		nTracks{NTracks},
		fV2{FV2} {}
		


//class for "MyTrack"
class MyTrack : public TObject
{
	public:
		Double_t fPhi;
		MyTrack();
		MyTrack(Double_t FPhi);
		
		ClassDef(MyTrack, 1);
};

ClassImp(MyTrack)

MyTrack::MyTrack():
	TObject(),
		fPhi(-1) {}

MyTrack::MyTrack(Double_t FPhi):
	TObject(),
	fPhi{FPhi} {}
#endif		
