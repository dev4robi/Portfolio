<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://www.springframework.org/tags" prefix="spring" %>
<html>
<head>
	<title><spring:message code="name.page.error"/></title>
</head>
<body>
	<h1><spring:message code="name.page.error"/>!</h1>
	<h2><spring:message code="error.code" arguments="${error.code}"/></h2>
	<p><a href="<c:url value='/main'/>">[<spring:message code="name.page.main"/>]</a></p>
</body>
</html>