package com.evtlogger.dao;

import java.util.Date;

import org.slf4j.Logger;

import com.fasterxml.jackson.annotation.JsonFormat;

// DB내 세부 컬럼 속성은 'WEB-INF/docs/TableSchemes.txt' 참고!
public class EventDao {
	private long evtSeq;                       // 경조사 시퀀스
	@JsonFormat(pattern="yyyy-MM-dd HH:mm:ss")
	private Date evtDate;                      // 경조사 일자
	private int evtWriteUserSeq;               // 경조사 작성자 시퀀스
	private int evtTgtUserSeq;                 // 경조사 대상 시퀀스
	private String evtTgtUserName;             // 경조사 대상 이름
	private String evtTgtUserPhoneNum;         // 경조사 대상 연락처
	private int evtTypeCode;                   // 경조사 종류 코드
	private int relationTypeCode;              // 경조사 대상과의 관계 코드
	private long payAmount;                    // 지불 금액
	private String evtMemo;                    // 경조사 메모
	
	public boolean validate(Logger logger) {
		if (evtDate == null) {
			logger.error("'evtDate' is null!");
			return false;
		}
		if (evtWriteUserSeq == 0) {
			logger.error("'evtWriterSeq' is zero!");
			return false;
		}
		if (evtTgtUserName == null || evtTgtUserName.length() == 0) {
			logger.error("'evtTgtUserName' is null of zero-length! (evtTgtUserName:'" + evtTgtUserName + "')");
			return false;
		}
		if (evtTgtUserPhoneNum == null || evtTgtUserPhoneNum.length() == 0) {
			logger.error("'evtTgtUserPhoneNum' is null of zero-length! (evtTgtUserPhoneNum:'" + evtTgtUserPhoneNum + "')");
			return false;
		}
		if (evtTypeCode == 0) {
			logger.error("'evtTypeCode' is zero!");
			return false;
		}
		if (relationTypeCode == 0) {
			logger.error("'evtTypeCode' is zero!");
			return false;
		}
		if (evtMemo == null) {
			logger.error("'evtMemo' is null!");
			return false;
		}
		
		return true;
	}
	
	public long getEvtSeq() {
		return evtSeq;
	}
	public void setEvtSeq(long evtSeq) {
		this.evtSeq = evtSeq;
	}
	public Date getEvtDate() {
		return evtDate;
	}
	public void setEvtDate(Date evtWriteDate) {
		this.evtDate = evtWriteDate;
	}
	public int getEvtWriterUserSeq() {
		return evtWriteUserSeq;
	}
	public void setEvtWriterUserSeq(int evtWriteUserSeq) {
		this.evtWriteUserSeq = evtWriteUserSeq;
	}
	public int getEvtTgtUserSeq() {
		return evtTgtUserSeq;
	}
	public void setEvtTgtUserSeq(int evtTgtUserSeq) {
		this.evtTgtUserSeq = evtTgtUserSeq;
	}
	public String getEvtTgtUserName() {
		return evtTgtUserName;
	}
	public void setEvtTgtUserName(String evtTgtUserName) {
		this.evtTgtUserName = evtTgtUserName;
	}
	public String getEvtTgtUserPhoneNum() {
		return evtTgtUserPhoneNum;
	}
	public void setEvtTgtUserPhoneNum(String evtTgtUserPhoneNum) {
		this.evtTgtUserPhoneNum = evtTgtUserPhoneNum;
	}
	public int getEvtTypeCode() {
		return evtTypeCode;
	}
	public void setEvtTypeCode(int evtTypeCode) {
		this.evtTypeCode = evtTypeCode;
	}
	public int getRelationTypeCode() {
		return relationTypeCode;
	}
	public void setRelationTypeCode(int relationTypeCode) {
		this.relationTypeCode = relationTypeCode;
	}
	public long getPayAmount() {
		return payAmount;
	}
	public void setPayAmount(long payAmount) {
		this.payAmount = payAmount;
	}
	public String getEvtMemo() {
		return evtMemo;
	}
	public void setEvtMemo(String evtMemo) {
		this.evtMemo = evtMemo;
	}
}
