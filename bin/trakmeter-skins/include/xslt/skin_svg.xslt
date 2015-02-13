<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<xsl:stylesheet version="2.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://www.w3.org/2005/04/xpath-functions" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:xlink="http://www.w3.org/1999/xlink" exclude-result-prefixes="xsl fn xs">

	<xsl:output method="html" indent="yes" encoding="UTF-8" doctype-public="http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd" media-type="text/xhtml" omit-xml-declaration="no" version="4.01" />


	<xsl:template match="/">
		<html lang="en">
			<head>
				<title><xsl:value-of select="/*/@name" /></title>

				<style type="text/css">
					<xsl:choose>
						<xsl:when test="/*/@orientation = 'vertical'">
							th { text-align: center; vertical-align: top; font-weight: bold; }
							th span { font-weight: normal; }
							td { text-align: center; vertical-align: top; font-weight: bold; }
						</xsl:when>

						<xsl:otherwise>
							th { text-align: left; vertical-align: top; font-weight: bold; }
							th span { font-weight: normal; }
							td { text-align: left; vertical-align: top; font-weight: bold; }
						</xsl:otherwise>
					</xsl:choose>
				</style>
			</head>

			<body>
				<h1>TraKmeter Skin &quot;<xsl:value-of select="/*/@name" />&quot;</h1>

				<xsl:call-template name="draw_meters">
					<xsl:with-param name="configuration" select="'stereo'" />
					<xsl:with-param name="meter-mode" select="'split'" />
					<xsl:with-param name="fallback_1" select="/*/stereo_split" />
					<xsl:with-param name="fallback_2" select="/*/default" />
				</xsl:call-template>

				<xsl:call-template name="draw_meters">
					<xsl:with-param name="configuration" select="'stereo'" />
					<xsl:with-param name="meter-mode" select="'combined'" />
					<xsl:with-param name="fallback_1" select="/*/stereo_combined" />
					<xsl:with-param name="fallback_2" select="/*/default" />
				</xsl:call-template>

				<xsl:call-template name="draw_meters">
					<xsl:with-param name="configuration" select="'multi'" />
					<xsl:with-param name="meter-mode" select="'split'" />
					<xsl:with-param name="fallback_1" select="/*/multi_split" />
					<xsl:with-param name="fallback_2" select="/*/default" />
				</xsl:call-template>

				<xsl:call-template name="draw_meters">
					<xsl:with-param name="configuration" select="'multi'" />
					<xsl:with-param name="meter-mode" select="'combined'" />
					<xsl:with-param name="fallback_1" select="/*/multi_combined" />
					<xsl:with-param name="fallback_2" select="/*/default" />
				</xsl:call-template>
			</body>
		</html>
	</xsl:template>


	<xsl:template name="draw_meters">
		<xsl:param name="configuration" />
		<xsl:param name="meter-mode" />
		<xsl:param name="fallback_1" />
		<xsl:param name="fallback_2" />

		<xsl:variable name="meter_mode_label">
			<xsl:choose>
				<xsl:when test="$meter-mode = 'combined'">Combined</xsl:when>

				<xsl:otherwise>Split</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<h2>
			<xsl:value-of select="concat(upper-case(substring($configuration, 1, 1)), substring($configuration, 2)), ' ('" />
			<xsl:value-of select="$meter_mode_label" />)
		</h2>

		<xsl:choose>
			<xsl:when test="/*/@orientation = 'vertical'">
				<table>
					<tbody>
						<tr>
							<th>
								K-20
							</th>

							<th>
								Normal
							</th>
						</tr>

						<tr>
							<td>
								<xsl:call-template name="draw_single_meter">
									<xsl:with-param name="node" select="root($fallback_2)/*/*[local-name() = string-join(($configuration, $meter-mode, 'k20'), '_')]" />
									<xsl:with-param name="fallback_1" select="$fallback_1" />
									<xsl:with-param name="fallback_2" select="$fallback_2" />
									<xsl:with-param name="configuration" select="$configuration" />
									<xsl:with-param name="meter-mode" select="$meter-mode" />
									<xsl:with-param name="crest-factor" select="'k20'" />
								</xsl:call-template>
							</td>

							<td>
								<xsl:call-template name="draw_single_meter">
									<xsl:with-param name="node" select="root($fallback_2)/*/*[local-name() = string-join(($configuration, $meter-mode, 'normal'), '_')]" />
									<xsl:with-param name="fallback_1" select="$fallback_1" />
									<xsl:with-param name="fallback_2" select="$fallback_2" />
									<xsl:with-param name="configuration" select="$configuration" />
									<xsl:with-param name="meter-mode" select="$meter-mode" />
									<xsl:with-param name="crest-factor" select="'normal'" />
								</xsl:call-template>
							</td>
						</tr>
					</tbody>
				</table>
			</xsl:when>

			<xsl:otherwise>
				<table>
					<tbody>
						<tr>
							<th>
								K-20
							</th>

							<td>
								<xsl:call-template name="draw_single_meter">
									<xsl:with-param name="node" select="root($fallback_2)/*/*[local-name() = string-join(($configuration, $meter-mode, 'k20'), '_')]" />
									<xsl:with-param name="fallback_1" select="$fallback_1" />
									<xsl:with-param name="fallback_2" select="$fallback_2" />
									<xsl:with-param name="configuration" select="$configuration" />
									<xsl:with-param name="meter-mode" select="$meter-mode" />
									<xsl:with-param name="crest-factor" select="'k20'" />
								</xsl:call-template>
							</td>

							<tr>
								<th>
									Normal
								</th>

								<td>
									<xsl:call-template name="draw_single_meter">
										<xsl:with-param name="node" select="root($fallback_2)/*/*[local-name() = string-join(($configuration, $meter-mode, 'normal'), '_')]" />
										<xsl:with-param name="fallback_1" select="$fallback_1" />
										<xsl:with-param name="fallback_2" select="$fallback_2" />
										<xsl:with-param name="configuration" select="$configuration" />
										<xsl:with-param name="meter-mode" select="$meter-mode" />
										<xsl:with-param name="crest-factor" select="'normal'" />
									</xsl:call-template>
								</td>
							</tr>
						</tr>
					</tbody>
				</table>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>


	<xsl:template name="draw_single_meter">
		<xsl:param name="node" />
		<xsl:param name="fallback_1" />
		<xsl:param name="fallback_2" />
		<xsl:param name="configuration" />
		<xsl:param name="meter-mode" />
		<xsl:param name="crest-factor" />

		<svg xmlns="http://www.w3.org/2000/svg" version="1.1" width="{$node/background/@width}" height="{$node/background/@height}">
			<rect x="0" y="0" width="{$node/background/@width}" height="{$node/background/@height}" style="fill:silver;" />

			<image x="0" y="0" width="{$node/background/@width}" height="{$node/background/@height}">
			  <xsl:attribute name="xlink:href"><xsl:value-of select="concat(root($node)/*/@path, '/', $node/background/@image)" /></xsl:attribute>
			</image>

			<xsl:for-each select="$node/meter_graduation">
				<image x="{@x}" y="{@y}" width="{@width}" height="{@height}">
				  <xsl:attribute name="xlink:href"><xsl:value-of select="concat(root($node)/*/@path, '/', @image)" /></xsl:attribute>
				</image>
			</xsl:for-each>

			<xsl:for-each select="tokenize('button_split | button_k20 | button_transient | button_mixing | button_skin | button_reset | button_validate | button_about', '\s*\|\s*')">
				<xsl:variable name="button">
					<xsl:value-of select="string(.)" />
				</xsl:variable>

				<xsl:call-template name="component_active">
					<xsl:with-param name="node" select="$node/*[local-name() = $button]" />
					<xsl:with-param name="fallback_1" select="$fallback_1/*[local-name() = $button]" />
					<xsl:with-param name="fallback_2" select="$fallback_2/*[local-name() = $button]" />
					<xsl:with-param name="component_state">
						<xsl:choose>
							<xsl:when test="$button = 'button_split'">
								<xsl:choose>
									<xsl:when test="ends-with($button, $meter-mode)">on</xsl:when>

									<xsl:otherwise>off</xsl:otherwise>
								</xsl:choose>
							</xsl:when>

							<xsl:when test="$button = 'button_k20'">
								<xsl:choose>
									<xsl:when test="ends-with($button, $crest-factor)">on</xsl:when>

									<xsl:otherwise>off</xsl:otherwise>
								</xsl:choose>
							</xsl:when>

							<xsl:when test="$crest-factor = 'k20'">off</xsl:when>

							<xsl:otherwise>on</xsl:otherwise>
						</xsl:choose>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:for-each>

			<xsl:if test="$crest-factor = 'normal'">
				<xsl:for-each select="tokenize('slider_gain', '\s*\|\s*')">
					<xsl:variable name="slider">
						<xsl:value-of select="string(.)" />
					</xsl:variable>

					<xsl:call-template name="component_meter">
						<xsl:with-param name="node" select="$node/*[local-name() = $slider]" />
						<xsl:with-param name="fallback_1" select="$fallback_1/*[local-name() = $slider]" />
						<xsl:with-param name="fallback_2" select="$fallback_2/*[local-name() = $slider]" />
					</xsl:call-template>
				</xsl:for-each>
			</xsl:if>

			<xsl:for-each select="tokenize('label_over_1 | label_over_2 | label_over_3 | label_over_4 | label_over_5 | label_over_6 | label_over_7 | label_over_8', '\s*\|\s*')">
				<xsl:variable name="label">
					<xsl:value-of select="string(.)" />
				</xsl:variable>

				<xsl:call-template name="component_active">
					<xsl:with-param name="node" select="$node/*[local-name() = $label]" />
					<xsl:with-param name="fallback_1" select="$fallback_1/*[local-name() = $label]" />
					<xsl:with-param name="fallback_2" select="$fallback_2/*[local-name() = $label]" />

					<xsl:with-param name="component_state">
						<xsl:choose>
							<xsl:when test="ends-with($label, '_1')">on</xsl:when>

							<xsl:when test="ends-with($label, '_3')">active</xsl:when>

							<xsl:otherwise>off</xsl:otherwise>
						</xsl:choose>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:for-each>

			<xsl:for-each select="tokenize('meter_peak_1 | meter_peak_2 | meter_peak_3 | meter_peak_4 | meter_peak_5 | meter_peak_6 | meter_peak_7 | meter_peak_8 | meter_average_1 | meter_average_2 | meter_average_3 | meter_average_4 | meter_average_5 | meter_average_6 | meter_average_7 | meter_average_8 | label_signal_1 | label_signal_2 | label_signal_3 | label_signal_4 | label_signal_5 | label_signal_6 | label_signal_7 | label_signal_8', '\s*\|\s*')">
				<xsl:variable name="meter">
					<xsl:value-of select="string(.)" />
				</xsl:variable>

				<xsl:call-template name="component_meter">
					<xsl:with-param name="node" select="$node/*[local-name() = $meter]" />
					<xsl:with-param name="fallback_1" select="$fallback_1/*[local-name() = $meter]" />
					<xsl:with-param name="fallback_2" select="$fallback_2/*[local-name() = $meter]" />
				</xsl:call-template>
			</xsl:for-each>

			<xsl:if test="$crest-factor = 'k20'">
				<xsl:for-each select="tokenize('label_debug', '\s*\|\s*')">
					<xsl:variable name="label">
						<xsl:value-of select="string(.)" />
					</xsl:variable>

					<xsl:call-template name="component_passive">
						<xsl:with-param name="node" select="$node/*[local-name() = $label]" />
						<xsl:with-param name="fallback_1" select="$fallback_1/*[local-name() = $label]" />
						<xsl:with-param name="fallback_2" select="$fallback_2/*[local-name() = $label]" />
					</xsl:call-template>
				</xsl:for-each>
			</xsl:if>
		</svg>
	</xsl:template>


	<xsl:template name="component_active">
		<xsl:param name="node" />
		<xsl:param name="fallback_1" />
		<xsl:param name="fallback_2" />
		<xsl:param name="component_state" />

		<xsl:choose>
			<xsl:when test="exists($node)">
				<image x="{$node/@x}" y="{$node/@y}" width="{$node/@width}" height="{$node/@height}">
					<xsl:choose>
						<xsl:when test="$component_state = 'on'">
							<xsl:attribute name="xlink:href"><xsl:value-of select="concat(root($node)/*/@path, '/', ($node/@image_on))" /></xsl:attribute>
						</xsl:when>

						<xsl:when test="$component_state = 'active'">
							<xsl:attribute name="xlink:href"><xsl:value-of select="concat(root($node)/*/@path, '/', ($node/@image_active))" /></xsl:attribute>
						</xsl:when>

						<xsl:otherwise>
							<xsl:attribute name="xlink:href"><xsl:value-of select="concat(root($node)/*/@path, '/', ($node/@image_off))" /></xsl:attribute>
						</xsl:otherwise>
					</xsl:choose>
				</image>
			</xsl:when>

			<xsl:otherwise>
				<xsl:choose>
					<xsl:when test="exists($fallback_1)">
						<image x="{$fallback_1/@x}" y="{$fallback_1/@y}" width="{$fallback_1/@width}" height="{$fallback_1/@height}">
							<xsl:choose>
								<xsl:when test="$component_state = 'on'">
									<xsl:attribute name="xlink:href"><xsl:value-of select="concat(root($fallback_1)/*/@path, '/', ($fallback_1/@image_on))" /></xsl:attribute>
								</xsl:when>

								<xsl:when test="$component_state = 'active'">
									<xsl:attribute name="xlink:href"><xsl:value-of select="concat(root($fallback_1)/*/@path, '/', ($fallback_1/@image_active))" /></xsl:attribute>
								</xsl:when>

								<xsl:otherwise>
									<xsl:attribute name="xlink:href"><xsl:value-of select="concat(root($fallback_1)/*/@path, '/', ($fallback_1/@image_off))" /></xsl:attribute>
								</xsl:otherwise>
							</xsl:choose>
						</image>
					</xsl:when>

					<xsl:otherwise>
						<image x="{$fallback_2/@x}" y="{$fallback_2/@y}" width="{$fallback_2/@width}" height="{$fallback_2/@height}">
							<xsl:choose>
								<xsl:when test="$component_state = 'on'">
									<xsl:attribute name="xlink:href"><xsl:value-of select="concat(root($fallback_2)/*/@path, '/', ($fallback_2/@image_on))" /></xsl:attribute>
								</xsl:when>

								<xsl:when test="$component_state = 'active'">
									<xsl:attribute name="xlink:href"><xsl:value-of select="concat(root($fallback_2)/*/@path, '/', ($fallback_2/@image_active))" /></xsl:attribute>
								</xsl:when>

								<xsl:otherwise>
									<xsl:attribute name="xlink:href"><xsl:value-of select="concat(root($fallback_2)/*/@path, '/', ($fallback_2/@image_off))" /></xsl:attribute>
								</xsl:otherwise>
							</xsl:choose>
						</image>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>


	<xsl:template name="component_passive">
		<xsl:param name="node" />
		<xsl:param name="fallback_1" />
		<xsl:param name="fallback_2" />

		<xsl:choose>
			<xsl:when test="exists($node)">
				<image x="{$node/@x}" y="{$node/@y}" width="{$node/@width}" height="{$node/@height}">
					<xsl:attribute name="xlink:href"><xsl:value-of select="concat(root($node)/*/@path, '/', ($node/@image))" /></xsl:attribute>
				</image>
			</xsl:when>

			<xsl:otherwise>
				<xsl:choose>
					<xsl:when test="exists($fallback_1)">
						<image x="{$fallback_1/@x}" y="{$fallback_1/@y}" width="{$fallback_1/@width}" height="{$fallback_1/@height}">
							<xsl:attribute name="xlink:href"><xsl:value-of select="concat(root($fallback_1)/*/@path, '/', ($fallback_1/@image))" /></xsl:attribute>
						</image>
					</xsl:when>

					<xsl:otherwise>
						<image x="{$fallback_2/@x}" y="{$fallback_2/@y}" width="{$fallback_2/@width}" height="{$fallback_2/@height}">
							<xsl:attribute name="xlink:href"><xsl:value-of select="concat(root($fallback_2)/*/@path, '/', ($fallback_2/@image))" /></xsl:attribute>
						</image>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>


	<xsl:template name="component_meter">
		<xsl:param name="node" />
		<xsl:param name="fallback_1" />
		<xsl:param name="fallback_2" />

		<xsl:choose>
			<xsl:when test="exists($node)">
				<image x="{$node/@x}" y="{$node/@y}" width="{$node/@width}" height="{$node/@height}">
				  <xsl:attribute name="xlink:href"><xsl:value-of select="concat(root($node)/*/@path, '/', ($node/@image))" /></xsl:attribute>
				</image>
			</xsl:when>

			<xsl:otherwise>
				<xsl:choose>
					<xsl:when test="exists($fallback_1)">
						<image x="{$fallback_1/@x}" y="{$fallback_1/@y}" width="{$fallback_1/@width}" height="{$fallback_1/@height}">
							<xsl:attribute name="xlink:href"><xsl:value-of select="concat(root($fallback_1)/*/@path, '/', ($fallback_1/@image))" /></xsl:attribute>
						</image>
					</xsl:when>

					<xsl:otherwise>
						<image x="{$fallback_2/@x}" y="{$fallback_2/@y}" width="{$fallback_2/@width}" height="{$fallback_2/@height}">
							<xsl:attribute name="xlink:href"><xsl:value-of select="concat(root($fallback_2)/*/@path, '/', ($fallback_2/@image))" /></xsl:attribute>
						</image>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

</xsl:stylesheet>
