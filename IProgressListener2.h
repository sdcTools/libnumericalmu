#ifndef I_ProgressListener_h
#define I_ProgressListener_h

class IProgressListener2
{
public:
	virtual ~IProgressListener2() { }
	virtual void UpdateProgress(int perc) = 0;
};

#endif // I_ProgressListener_h
