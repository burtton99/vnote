#include "plantumlhelper.h"

#include <QDebug>

#include <utils/processutils.h>

using namespace vnotex;

PlantUmlHelper::PlantUmlHelper()
{
}

void PlantUmlHelper::init(const QString &p_plantUmlJarFile,
                          const QString &p_graphvizFile,
                          const QString &p_overriddenCommand)
{
    qDebug() << "PlantUmlHelper" << p_plantUmlJarFile << p_graphvizFile << p_overriddenCommand;

    m_overriddenCommand = p_overriddenCommand;
    if (m_overriddenCommand.isEmpty()) {
        prepareProgramAndArgs(p_plantUmlJarFile, p_graphvizFile, m_program, m_args);
    } else {
        m_program.clear();
        m_args.clear();
    }
}

void PlantUmlHelper::prepareProgramAndArgs(const QString &p_plantUmlJarFile,
                                           const QString &p_graphvizFile,
                                           QString &p_program,
                                           QStringList &p_args)
{
#if defined(Q_OS_WIN)
    p_program = "java";
#else
    p_program = "/bin/sh";
    p_args << "-c";
    p_args << "java";
#endif

    p_args << "-Djava.awt.headless=true";

    p_args << "-jar" << p_plantUmlJarFile;

    p_args << "-charset" << "UTF-8";

    if (!p_graphvizFile.isEmpty()) {
        p_args << "-graphvizdot" << p_graphvizFile;
    }

    p_args << "-pipe";
}

QPair<bool, QString> PlantUmlHelper::testPlantUml(const QString &p_plantUmlJarFile)
{
    auto ret = qMakePair(false, QString());

    QString program;
    QStringList args;
    prepareProgramAndArgs(p_plantUmlJarFile, QString(), program, args);

    args << "-tsvg";
    args = getArgsToUse(args);

    const QString testGraph("VNote->Markdown : Hello");

    int exitCode = -1;
    QByteArray outData;
    QByteArray errData;
    auto state = ProcessUtils::start(program,
                                     args,
                                     testGraph.toUtf8(),
                                     exitCode,
                                     outData,
                                     errData);
    ret.first = (state == ProcessUtils::Succeeded) && (exitCode == 0);

    ret.second = QString("%1 %2\n\nExitcode: %3\n\nOutput: %4\n\nError: %5")
                        .arg(program, args.join(' '), QString::number(exitCode), QString::fromLocal8Bit(outData), QString::fromLocal8Bit(errData));

    return ret;
}

QStringList PlantUmlHelper::getArgsToUse(const QStringList &p_args)
{
    if (p_args.isEmpty()) {
        return QStringList();
    }

    if (p_args[0] == "-c") {
        // Combine all the arguments except the first one.
        QStringList args;
        args << p_args[0];

        QString subCmd;
        for (int i = 1; i < p_args.size(); ++i) {
            subCmd += " " + p_args[i];
        }
        args << subCmd;

        return args;
    } else {
        return p_args;
    }
}
