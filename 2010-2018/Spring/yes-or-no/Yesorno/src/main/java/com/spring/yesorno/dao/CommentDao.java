package com.spring.yesorno.dao;

import org.mybatis.spring.SqlSessionTemplate;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.dao.DataAccessException;

import com.spring.yesorno.dto.CommentDto;

public class CommentDao implements ICommentDao {

	@Autowired private SqlSessionTemplate sqlSession;
	
	public int insertComment(CommentDto writeCommentDto) throws DataAccessException {
		ICommentDao commentDao = sqlSession.getMapper(ICommentDao.class);
		return commentDao.insertComment(writeCommentDto);
	}
}
