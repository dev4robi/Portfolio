<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://www.springframework.org/tags" prefix="spring" %>
<html>
<head>
	<title><spring:message code="name.page.main"/></title>
</head>
<body>
	<h1><spring:message code="name.page.main"/></h1>
	<p><a href="main">[메인]</a></p>
	<p><a href="boards/voteboards/list/1">[찬반게시판]</a></p>

	<c:choose>
		<c:when test="${empty loginMember.memberToken}">
			<p><a href="members/register">[<spring:message code="members.register.title"/>]</a>
			<p><a href="members/login">[<spring:message code="members.login.title"/>]</a>	
		</c:when>
		<c:otherwise>
			<p><a href="members/logout">[<spring:message code="members.logout.title"/>]</a>
			<p><a href="members/infochange">[<spring:message code="members.changeinfo.title"/>]</a>
			<p><a href="members/deregister">[<spring:message code="members.deregister.title"/>]</a>

			<!-- TEST -->
			<p>memberId : ${loginMember.memberId}</p>
			<p>memberEmail : ${loginMember.memberEmail}</p>
			<p>memberNickname : ${loginMember.memberNickname}</p>
			<p>memberToken : ${loginMember.memberToken}</p>	
		</c:otherwise>
	</c:choose>
</body>
</html>