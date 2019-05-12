package com.evtlogger.mapper;

import java.util.Date;

import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;

import com.evtlogger.dao.UserDao;

@Mapper
public interface UserMapper {
	// 유저정보 등록
	public void insert_user(UserDao userDao);
	// 유저정보 획득
	public UserDao select_user(@Param("userId") String userId);
	// 유저 로그인
	public void update_user_login(@Param("userId") String userId,
							      @Param("userLastLoginDate") Date userLastLoginDate);
	// 유저 회원탈퇴
	public void update_user_signout(@Param("userId") String userId,
									@Param("userSignoutDate") Date userSignoutDate);
}
