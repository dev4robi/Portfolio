package com.spring.yesorno.dto;

import java.util.Date;

public class CommentDto {

	private int commentId;				// INT			PK,AI		댓글 번호
	private int commentOwnedBoardId;	// INT			FK(ODC),NN	댓글 작성 게시글 번호
	private int commentWriterMemberId;	// INT			FK(ODR),NN	댓글 작성자 번호
	private Date commentWrittenDate;	// DATETIME		NN			댓글 작성일
	private String commentContent;		// VARCHAR(256)	NN			댓글 내용
	private int commentBaseCnt;			// INT			NN			대댓글의 덧글번호  
	private int commentStepCnt;			// INT			NN			대댓글 순번
	
	public int getCommentId() {
		return commentId;
	}
	
	public void setCommentId(int commentId) {
		this.commentId = commentId;
	}
	
	public int getCommentOwnedBoardId() {
		return commentOwnedBoardId;
	}
	
	public void setCommentOwnedBoardId(int commentOwnedBoardId) {
		this.commentOwnedBoardId = commentOwnedBoardId;
	}
	
	public int getCommentWriterMemberId() {
		return commentWriterMemberId;
	}
	
	public void setCommentWriterMemberId(int commentWriterMemberId) {
		this.commentWriterMemberId = commentWriterMemberId;
	}
	
	public Date getCommentWrittenDate() {
		return commentWrittenDate;
	}
	
	public void setCommentWrittenDate(Date commentWrittenDate) {
		this.commentWrittenDate = commentWrittenDate;
	}
	
	public String getCommentContent() {
		return commentContent;
	}
	
	public void setCommentContent(String commentContent) {
		this.commentContent = commentContent;
	}
	
	public int getCommentBaseCnt() {
		return commentBaseCnt;
	}
	
	public void setCommentBaseCnt(int commentBaseCnt) {
		this.commentBaseCnt = commentBaseCnt;
	}
	
	public int getCommentStepCnt() {
		return commentStepCnt;
	}
	
	public void setCommentStepCnt(int commentStepCnt) {
		this.commentStepCnt = commentStepCnt;
	}
}
