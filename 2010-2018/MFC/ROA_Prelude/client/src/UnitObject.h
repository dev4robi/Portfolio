class UnitObject
{
	protected:
	
	public:
		Unit Object;
	
	public:
		UnitObject(int uID=0, int uX=0, int uY=0, int uDirection=0, int uMotion=0);
		~UnitObject();

		void AtkUnit(); // ������ �Ϲ����� ���ݽ�Ű�� �Լ�
		BOOL MoveUnit(); // ������ �Ϲ������� �̵���Ű�� �Լ�
		void GiveDamageUnit(); // ���ֿ��� �������� �ִ� �Լ�
		void TeleportUnit(int x, int y, int Direction); // ������ ��� �����̵���Ű�� �Լ�
		void DeathUnit(); // ������ ü���� 0 ������ ��� ���̴� �Լ�
		void ReviveUnit(); // ������ ü���� 0 ������ �� �ٽ� �һ���Ű�� �Լ�
};