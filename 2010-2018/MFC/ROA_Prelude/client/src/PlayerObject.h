class PlayerObject
{
	public:
		int CamX, CamY;
		BOOL CharWndOn;
	
	public:
		PlayerObject();
		~PlayerObject();
		void MoveCamera(int x, int y);
};
