package com.evtlogger.dto;

public class DeleteEventDto {

	private String userJwt;
	private long evtSeq;
	
	public String getUserJwt() {
		return userJwt;
	}
	public void setUserJwt(String userJwt) {
		this.userJwt = userJwt;
	}
	public long getEvtSeq() {
		return evtSeq;
	}
	public void setEvtSeq(long evtSeq) {
		this.evtSeq = evtSeq;
	}
}
