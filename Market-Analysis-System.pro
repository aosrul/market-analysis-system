###############################################################################
#                                                                             #
#   MAS: Market Analysis System Project                                       #
#   web.site.com                                                              #
#                                                                             #
#   M A R K E T   A N A L Y S I S   S Y S T E M   P R O J E C T               #
#                                                                             #
#   Aleksey Terentew                                                          #
#   My company                                                                #
#   terentew.aleksey@ya.ru                                                    #
#                                                                             #
###############################################################################

# CONFIGURATION

TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += Market-Analysis
SUBDIRS += Plugins

app.depends = plugins
