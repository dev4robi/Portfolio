package com.evtlogger.dao;

//DB내 세부 컬럼 속성은 'WEB-INF/docs/TableSchemes.txt' 참고!
public class TypeCodeDao {

	public int tcSeq;			// 코드 시퀀스 (코드값)
	public String tcName;		// 코드 이름
	
	public int getTcSeq() {
		return tcSeq;
	}
	public void setTcSeq(int tcSeq) {
		this.tcSeq = tcSeq;
	}
	public String getTcName() {
		return tcName;
	}
	public void setTcName(String tcName) {
		this.tcName = tcName;
	}
}
