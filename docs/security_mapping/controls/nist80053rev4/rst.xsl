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
    <xsl:template match="c:statement">
        <xsl:text>&#xa;&#xa;</xsl:text>
        <xsl:text>.. _</xsl:text>
        <xsl:value-of select="c:number"/>:
        <xsl:text>&#xa;&#xa;</xsl:text>
        <xsl:value-of select="c:number"/>
        <xsl:text>&#xa;''''''''''''''''''''''''''''''''''''''''''''''''''''''&#xa;&#xa;</xsl:text>
        <xsl:text>&#xa;</xsl:text>
        <xsl:text>Description:  </xsl:text>
        <xsl:text>"</xsl:text><xsl:value-of select="c:description"/><xsl:text>"</xsl:text>
        <xsl:text>&#xa;</xsl:text>
        <xsl:apply-templates select="c:statement"/>
    </xsl:template>
    
    <xsl:template name="control-info">
        <xsl:text>&#xa;</xsl:text>
        <xsl:text>.. _</xsl:text>
        <xsl:value-of select="c:number"/>:
        <xsl:text>&#xa;&#xa;</xsl:text>
        <xsl:value-of select="c:number"/>
        <xsl:text>&#xa;````````````````````````````````````````````````````&#xa;&#xa;</xsl:text>
        <xsl:value-of select="c:number"/>
        <xsl:text> : </xsl:text>
        <xsl:value-of select="c:title"/>
        <xsl:text>&#xa;&#xa;</xsl:text>
        <xsl:text>Priority:  </xsl:text>
        <xsl:value-of select="c:priority"/>
        <xsl:text>&#xa;&#xa;</xsl:text>
        <xsl:text>Baseline-Impact:  </xsl:text>
        <xsl:value-of select="string-join(c:baseline-impact,',')"/>
        <xsl:text>&#xa;&#xa;</xsl:text>
        <xsl:text>Description:  </xsl:text>
        <xsl:text>"</xsl:text><xsl:value-of select="c:statement/c:description"/><xsl:text>"</xsl:text>
        <xsl:text>&#xa;</xsl:text>
        <xsl:text>Supplemental Guidance:  </xsl:text>
        <xsl:if test="c:supplemental-guidance/c:description">
            <xsl:text>"</xsl:text><xsl:value-of select="replace(c:supplemental-guidance/c:description, '&#xa;', ' ')"/><xsl:text>"</xsl:text>
        </xsl:if>
        <xsl:text>&#xa;&#xa;</xsl:text>
        <xsl:text>Related Controls:</xsl:text>
        <xsl:value-of select="string-join(c:supplemental-guidance/c:related,',')"/>
        <xsl:text>&#xa;</xsl:text>
    </xsl:template>
    
    <xsl:template match="/controls:controls">
      <!-- Add this line to stop the left menu from expanding each control -->
      <xsl:text>:tocdepth: 2&#xa;&#xa;</xsl:text>
      <xsl:text>NIST 800-53 Rev4</xsl:text>
      <xsl:text>&#xa;</xsl:text>
      <xsl:text>================</xsl:text>
      <xsl:text>&#xa;</xsl:text>
        <xsl:for-each select="controls:control">
            <!-- Controls -->
            <xsl:text>.. _header_</xsl:text>
            <xsl:value-of select="c:number"/>:<xsl:text>&#xa;&#xa;</xsl:text>
            <xsl:value-of select="c:number"/>
            <xsl:text>&#xa;-----------------------------------------------&#xa;</xsl:text>
            <xsl:text>&#xa;&#xa;</xsl:text>
            <xsl:text>Control Family:  </xsl:text>
            <xsl:variable name="family"><xsl:value-of select="c:family"/></xsl:variable>
            <xsl:value-of select="c:family"/>
            <xsl:text>&#xa;</xsl:text>
            <xsl:call-template name="control-info"/>
            <xsl:text>&#xa;&#xa;</xsl:text>
            <!-- Statements -->
            <xsl:apply-templates select="c:statement/c:statement"/>
            <!-- Control Enhancements -->
            <xsl:for-each select="c:control-enhancements/c:control-enhancement">
                <!--xsl:text>&#xa;</xsl:text-->
                <!--xsl:value-of select="$family"/-->
                <xsl:call-template name="control-info"/>
                <xsl:apply-templates select="c:statement/c:statement"/>
            </xsl:for-each>
        </xsl:for-each>
    </xsl:template>
</xsl:stylesheet>
