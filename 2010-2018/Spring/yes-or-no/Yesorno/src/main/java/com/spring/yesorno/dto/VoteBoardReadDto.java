package com.spring.yesorno.dto;

import java.util.Date;

/** 투표 게시글 읽기 내용 */
public class VoteBoardReadDto extends VoteBoardListDto {

	private boolean readerIsWriter;			// 글 작성자 확인
	private String writerMemberNickname;	// 투표 게시글 작성자 닉네임
	private Date voteBoardWrittenDate;		// 투표 게시글 작성일
	private int voteBoardViewCnt;			// 투표 게시글 조회수
	private String voteBoardContent;		// 누표 게시글 내용
	private Date voteEndDate;				// 투표 마감 날자
	private int voteEndCnt;					// 투표 마감 투표수
	private int voteAgreePercent;			// 투표 찬성 백분율

	public VoteBoardReadDto(VoteBoardListDto dto) {
		this.voteBoardId = dto.voteBoardId;
		this.voteBoardTitle = dto.voteBoardTitle;
		this.voteBoardImageURL = dto.voteBoardImageURL;
		this.voteAgreeCnt = dto.voteAgreeCnt;
		this.voteDisagreeCnt = dto.voteDisagreeCnt;
		this.voteBoardState = dto.voteBoardState;
	}
	
	public boolean isReaderIsWriter() {
		return readerIsWriter;
	}

	public void setReaderIsWriter(boolean readerIsWriter) {
		this.readerIsWriter = readerIsWriter;
	}

	public String getWriterMemberNickname() {
		return writerMemberNickname;
	}
	
	public void setWriterMemberNickname(String writerMemberNickname) {
		this.writerMemberNickname = writerMemberNickname;
	}
	
	public Date getVoteBoardWrittenDate() {
		return voteBoardWrittenDate;
	}
	
	public void setVoteBoardWrittenDate(Date voteBoardWrittenDate) {
		this.voteBoardWrittenDate = voteBoardWrittenDate;
	}
	
	public int getVoteBoardViewCnt() {
		return voteBoardViewCnt;
	}
	
	public void setVoteBoardViewCnt(int voteBoardViewCnt) {
		this.voteBoardViewCnt = voteBoardViewCnt;
	}
	
	public String getVoteBoardContent() {
		return voteBoardContent;
	}
	
	public void setVoteBoardContent(String voteBoardContent) {
		this.voteBoardContent = voteBoardContent;
	}

	public Date getVoteEndDate() {
		return voteEndDate;
	}
	
	public void setVoteEndDate(Date voteEndDate) {
		this.voteEndDate = voteEndDate;
	}
	
	public int getVoteEndCnt() {
		return voteEndCnt;
	}
	
	public void setVoteEndCnt(int voteEndCnt) {
		this.voteEndCnt = voteEndCnt;
	}

	public int getVoteAgreePercent() {
		return voteAgreePercent;
	}

	public void setVoteAgreePercent(int voteAgreePercent) {
		this.voteAgreePercent = voteAgreePercent;
	}
}
