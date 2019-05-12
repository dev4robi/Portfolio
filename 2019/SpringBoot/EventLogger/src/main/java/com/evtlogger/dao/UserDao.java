package com.evtlogger.dao;

import java.util.Date;

// DB내 세부 컬럼 속성은 'WEB-INF/docs/TableSchemes.txt' 참고!
public class UserDao {

	private int userSeq;				// 유저 시퀀스
	private String userId;				// 유저 아이디
	private String userName;			// 유저 이름
	private Date userJoinDate;		// 유저 가입 일자
	private Date userLastLoginDate;	// 유저 마지막 로그인 일자
	private Date userSignoutDate;	// 유저 탈퇴일자
	
	public int getUserSeq() {
		return userSeq;
	}
	public void setUserSeq(int userSeq) {
		this.userSeq = userSeq;
	}
	public String getUserId() {
		return userId;
	}
	public void setUserId(String userId) {
		this.userId = userId;
	}
	public String getUserName() {
		return userName;
	}
	public void setUserName(String userName) {
		this.userName = userName;
	}
	public Date getUserJoinDate() {
		return userJoinDate;
	}
	public void setUserJoinDate(Date userJoinDate) {
		this.userJoinDate = userJoinDate;
	}
	public Date getUserLastLoginDate() {
		return userLastLoginDate;
	}
	public void setUserLastLoginDate(Date userLastLoginDate) {
		this.userLastLoginDate = userLastLoginDate;
	}
	public Date getUserSignoutDate() {
		return userSignoutDate;
	}
	public void setUserSignoutDate(Date userSignoutDate) {
		this.userSignoutDate = userSignoutDate;
	}
}
