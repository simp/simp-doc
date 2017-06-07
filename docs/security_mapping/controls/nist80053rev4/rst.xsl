<?xml version="1.0" encoding="UTF-8"?>
<!-- This styleysheet transforms the NIST 800-53 to reStructuredText in a format that works for the SIMP documentation.  It's a modified version of the transform provided by NIST.-->
<!-- The saxon tool is used for the conversion:  java -jar /usr/share/java/saxon.jar -xsl:rst.xsl -s:800-53-controls.xml > 800-53.controls.rst-->
<xsl:stylesheet
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:c="http://scap.nist.gov/schema/sp800-53/2.0"
    xmlns:controls="http://scap.nist.gov/schema/sp800-53/feed/2.0"
    xmlns:xhtml="http://www.w3.org/1999/xhtml"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://scap.nist.gov/schema/sp800-53/feed/2.0 http://scap.nist.gov/schema/sp800-53/feed/2.0/sp800-53-feed_2.0.xsd"
    version="2.0">
    <xsl:output method="text" omit-xml-declaration="yes" />

    <!-- Repeat a character a given number of times -->
    <!-- Pulled from https://stackoverflow.com/questions/5089096/how-to-show-a-character-n-times-in-xslt -->
    <xsl:template name="repeat">
      <xsl:param name="output" />
      <xsl:param name="count" />

      <xsl:if test="$count &gt; 0">
        <xsl:value-of select="$output" />
        <xsl:call-template name="repeat">
          <xsl:with-param name="output" select="$output" />
          <xsl:with-param name="count" select="$count - 1" />
        </xsl:call-template>
      </xsl:if>
    </xsl:template>

    <xsl:template name="reflink">
      <xsl:param name="header" />

      <xsl:text>&#xa;</xsl:text>
      <xsl:text>.. _</xsl:text>
      <!-- Need to make this RST safe -->
      <xsl:value-of select="replace($header, ':', '_')"/>:
      <xsl:text>&#xa;</xsl:text>
    </xsl:template>

    <xsl:template match="c:statement">
        <xsl:call-template name="reflink">
          <xsl:with-param name="header" select="c:number"/>
        </xsl:call-template>

        <xsl:value-of select="c:number"/>
        <xsl:text>&#xa;</xsl:text>

        <xsl:variable name="header_char">"</xsl:variable>
        <xsl:call-template name="repeat">
          <xsl:with-param name="output" select="$header_char"/>
          <xsl:with-param name="count" select="string-length(c:number)"/>
        </xsl:call-template>

        <xsl:text>&#xa;&#xa;</xsl:text>
        <!--
        <xsl:text>**Description:** </xsl:text>
        -->
        <xsl:text>    </xsl:text>
        <xsl:value-of select="c:description"/>
        <xsl:text>&#xa;</xsl:text>
        <xsl:apply-templates select="c:statement"/>
    </xsl:template>

    <xsl:template name="control-info">
        <xsl:call-template name="reflink">
          <xsl:with-param name="header" select="c:number"/>
        </xsl:call-template>

        <xsl:variable name="header">
          <xsl:value-of select="c:number"/>
          <xsl:text> : </xsl:text>
          <xsl:value-of select="c:title"/>
        </xsl:variable>

        <xsl:value-of select="$header"/>
        <xsl:text>&#xa;</xsl:text>

        <xsl:variable name="header_char">"</xsl:variable>
        <xsl:call-template name="repeat">
          <xsl:with-param name="output" select="$header_char"/>
          <xsl:with-param name="count" select="string-length($header)"/>
        </xsl:call-template>

        <xsl:if test="string-length(c:priority)!=0">
          <xsl:text>&#xa;&#xa;</xsl:text>
          <xsl:text>**Priority:** </xsl:text>
          <xsl:value-of select="c:priority"/>
        </xsl:if>

        <xsl:if test="count(c:baseline-impact)!=0">
          <xsl:text>&#xa;&#xa;</xsl:text>
          <xsl:text>**Baseline-Impact:** </xsl:text>

          <xsl:for-each select="c:baseline-impact">
            <xsl:choose>
              <xsl:when test=". = 'MODERATE'">
                <xsl:text>*MODERATE*, </xsl:text>
              </xsl:when>
              <xsl:when test=". = 'HIGH'">
                <xsl:text>**HIGH**</xsl:text>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="."/>
                <xsl:text>, </xsl:text>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:for-each>

        </xsl:if>

        <xsl:text>&#xa;&#xa;</xsl:text>
        <!--
        <xsl:text>**Description:** </xsl:text>
        -->
        <xsl:text>    </xsl:text>
        <xsl:value-of select="c:statement/c:description"/>

        <xsl:if test="string-length(c:supplemental-guidance/c:description)!=0">
          <xsl:text>&#xa;&#xa;</xsl:text>
          <!--
          <xsl:text>**Supplemental Guidance:**</xsl:text>

          <xsl:text>&#xa;&#xa;</xsl:text>
          -->
          <xsl:if test="c:supplemental-guidance/c:description">
            <xsl:text>.. NOTE::</xsl:text>
            <xsl:text>&#xa;&#xa;</xsl:text>
            <xsl:text>   </xsl:text>
            <xsl:value-of select="replace(c:supplemental-guidance/c:description, '&#xa;', '&#xa;    ')"/>
          </xsl:if>
        </xsl:if>

        <xsl:if test="count(c:supplemental-guidance/c:related)!=0">
          <xsl:text>&#xa;&#xa;</xsl:text>
          <xsl:text>**Related Controls:** </xsl:text>

          <xsl:for-each select="c:supplemental-guidance/c:related">
            <xsl:choose>
              <xsl:when test="matches(., '\s')">
                <xsl:value-of select="."/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:text>`</xsl:text>
                <xsl:value-of select="."/>
                <xsl:text>`_</xsl:text>
                <xsl:if test="position() != last()">
                  <xsl:text>, </xsl:text>
                </xsl:if>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:for-each>
        </xsl:if>

        <xsl:text>&#xa;</xsl:text>
    </xsl:template>

    <xsl:template match="/controls:controls">
      <!-- Add this line to stop the left menu from expanding each control -->
      <xsl:text>:tocdepth: 2</xsl:text>
      <xsl:text>&#xa;&#xa;</xsl:text>

      <xsl:text>NIST 800-53 Rev4</xsl:text>
      <xsl:text>&#xa;</xsl:text>
      <xsl:text>================</xsl:text>
      <xsl:text>&#xa;</xsl:text>
        <xsl:for-each select="controls:control">
            <!-- Controls -->
            <xsl:text>&#xa;</xsl:text>
            <xsl:variable name="header">
              <xsl:text>Control Family:  </xsl:text>
              <xsl:value-of select="c:family"/>
            </xsl:variable>

            <xsl:value-of select="$header"/>
            <xsl:text>&#xa;</xsl:text>

            <xsl:variable name="header_char">-</xsl:variable>
            <xsl:call-template name="repeat">
              <xsl:with-param name="output" select="$header_char"/>
              <xsl:with-param name="count" select="string-length($header)"/>
            </xsl:call-template>

            <xsl:text>&#xa;</xsl:text>
            <xsl:call-template name="control-info"/>
            <xsl:text>&#xa;</xsl:text>
            <!-- Statements -->
            <xsl:apply-templates select="c:statement/c:statement"/>
            <!-- Control Enhancements -->
            <xsl:for-each select="c:control-enhancements/c:control-enhancement">
                <xsl:call-template name="control-info"/>
                <xsl:apply-templates select="c:statement/c:statement"/>
            </xsl:for-each>
        </xsl:for-each>
    </xsl:template>
</xsl:stylesheet>
