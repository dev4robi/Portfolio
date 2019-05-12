class UnitObject
{
	protected:
	
	public:
		Unit Object;
	
	public:
		UnitObject(int uID=0, int uX=0, int uY=0, int uDirection=0, int uMotion=0);
		~UnitObject();

		void AtkUnit(); // 유닛의 일반적인 공격시키는 함수
		BOOL MoveUnit(); // 유닛을 일반적으로 이동시키는 함수
		void GiveDamageUnit(); // 유닛에게 데미지를 주는 함수
		void TeleportUnit(int x, int y, int Direction); // 유닛을 즉시 공간이동시키는 함수
		void DeathUnit(); // 유닛의 체력이 0 이하일 경우 죽이는 함수
		void ReviveUnit(); // 유닛의 체력이 0 이하일 때 다시 소생시키는 함수
};