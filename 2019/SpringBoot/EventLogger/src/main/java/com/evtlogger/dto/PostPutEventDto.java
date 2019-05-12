package com.evtlogger.dto;

import com.evtlogger.dao.EventDao;

public class PostPutEventDto {

	private String userJwt;
	private EventDao eventDao;
	
	public String getUserJwt() {
		return userJwt;
	}
	public void setUserJwt(String userJwt) {
		this.userJwt = userJwt;
	}
	public EventDao getEventDao() {
		return eventDao;
	}
	public void setEventDao(EventDao eventDao) {
		this.eventDao = eventDao;
	}
}
