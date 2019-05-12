package com.spring.yesorno.dao;

import org.springframework.dao.DataAccessException;

import com.spring.yesorno.dto.CommentDto;

public interface ICommentDao {

	public int insertComment(CommentDto commentDto) throws DataAccessException; 
}
