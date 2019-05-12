package com.spring.yesorno.util;

import java.io.UnsupportedEncodingException;
import java.util.Date;

import com.spring.yesorno.dto.MemberDto;

import io.jsonwebtoken.Claims;
import io.jsonwebtoken.Jws;
import io.jsonwebtoken.Jwts;
import io.jsonwebtoken.SignatureAlgorithm;

public class JwtMemberAuth {

	public static final long TokenExpireMillisec = 1000 * 60 * (15); // (15)분
	public static final long TokenExpireMinute = TokenExpireMillisec / 1000;
	private final String keyHS256;

	public JwtMemberAuth(String keyHS256) {
		this.keyHS256 = keyHS256;
	}

	// 토큰 생성
	public String createMemberToken(MemberDto memberDto) {
		final Date isaDate = new Date();
		final Date expDate = new Date(isaDate.getTime() + TokenExpireMillisec);
		String jwt = null;

		try {
			jwt = Jwts.builder().setHeaderParam("typ", "JWT")
								.setHeaderParam("alg", "HS256")
							    .setIssuedAt(isaDate)
							    .setExpiration(expDate)
							    .setSubject("memberToken")
							    .claim("memberId", memberDto.getMemberId())
							    .claim("memberEmail", memberDto.getMemberEmail())
							    .claim("memberNickname", memberDto.getMemberNickname())
							    .signWith(SignatureAlgorithm.HS256, keyHS256.getBytes("UTF-8"))
							    .compact();
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
			jwt = null;
		}
		
		return jwt;
	}
	
	// 토큰 검증
	public boolean authMemberToken(String jwt) {
		boolean authResult = false;
		Jws<Claims> claims = null;

		try {
			// 토큰 검사 - 파싱이 가능하면 아래 5가지 예외에서 벗어난 상태
			claims = Jwts.parser().setSigningKey(keyHS256.getBytes("UTF-8")).parseClaimsJws(jwt);
			String authMemberEmail = claims.getBody().get("memberEmail", String.class);
			if (authMemberEmail != null && authMemberEmail.length() > 0) {
				authResult = true;
			}
		} catch (Exception e) {
			/* [Throws]
			1) ExpiredJwtException : 유효기간 초과
			2) UnsupportedJwtException : 일치하지 않는 특정 형식이나 구성
			3) MalformedJwtException : 올바르게 구성되지 않음
			4) SignatureException : 서명 확인 불가
			5) IllegalArgumentException : null이거나 빈칸 */
			e.printStackTrace();
			authResult = false;
		}

		return authResult;
	}
	
	// 토큰에서 맴버 이메일 추출
	public MemberDto getMemberFromToken(String jwt) {
		MemberDto memberDto = null;
		Jws<Claims> claims = null;

		try {
			if ((claims = Jwts.parser().setSigningKey(keyHS256.getBytes("UTF-8")).parseClaimsJws(jwt)) != null) {
				memberDto = new MemberDto();
				memberDto.setMemberId(claims.getBody().get("memberId", Integer.class));
				memberDto.setMemberEmail(claims.getBody().get("memberEmail", String.class));
				memberDto.setMemberNickname(claims.getBody().get("memberNickname", String.class));
			}
		} catch (Exception e) {
			e.printStackTrace();
			memberDto = null;
		}

		return memberDto;
	}
}

/*
	@SuppressWarnings("unchecked")
	try {
		claims = Jwts.parser().setSigningKey(keyHS256.getBytes("UTF-8"))
				 .parseClaimsJws(jwt);
		resultMap = (Map<String, Object>)claims.getBody().get("memberId");
		} catch (Exception e) {
			resultMap = null;
		}
*/
