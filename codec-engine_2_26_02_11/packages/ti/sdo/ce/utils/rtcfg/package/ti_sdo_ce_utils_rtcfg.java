/*
 *  Do not modify this file; it is automatically 
 *  generated and any modifications will be overwritten.
 *
 * @(#) xdc-u12
 */
import java.util.*;
import org.mozilla.javascript.*;
import xdc.services.intern.xsr.*;
import xdc.services.spec.*;

public class ti_sdo_ce_utils_rtcfg
{
    static final String VERS = "@(#) xdc-u12\n";

    static final Proto.Elm $$T_Bool = Proto.Elm.newBool();
    static final Proto.Elm $$T_Num = Proto.Elm.newNum();
    static final Proto.Elm $$T_Str = Proto.Elm.newStr();
    static final Proto.Elm $$T_Obj = Proto.Elm.newObj();

    static final Proto.Fxn $$T_Met = new Proto.Fxn(null, null, 0, -1, false);
    static final Proto.Map $$T_Map = new Proto.Map($$T_Obj);
    static final Proto.Arr $$T_Vec = new Proto.Arr($$T_Obj);

    static final XScriptO $$DEFAULT = Value.DEFAULT;
    static final Object $$UNDEF = Undefined.instance;

    static final Proto.Obj $$Package = (Proto.Obj)Global.get("$$Package");
    static final Proto.Obj $$Module = (Proto.Obj)Global.get("$$Module");
    static final Proto.Obj $$Instance = (Proto.Obj)Global.get("$$Instance");
    static final Proto.Obj $$Params = (Proto.Obj)Global.get("$$Params");

    static final Object $$objFldGet = Global.get("$$objFldGet");
    static final Object $$objFldSet = Global.get("$$objFldSet");
    static final Object $$proxyGet = Global.get("$$proxyGet");
    static final Object $$proxySet = Global.get("$$proxySet");
    static final Object $$delegGet = Global.get("$$delegGet");
    static final Object $$delegSet = Global.get("$$delegSet");

    Scriptable xdcO;
    Session ses;
    Value.Obj om;

    boolean isROV;
    boolean isCFG;

    Proto.Obj pkgP;
    Value.Obj pkgV;

    ArrayList<Object> imports = new ArrayList<Object>();
    ArrayList<Object> loggables = new ArrayList<Object>();
    ArrayList<Object> mcfgs = new ArrayList<Object>();
    ArrayList<Object> proxies = new ArrayList<Object>();
    ArrayList<Object> sizes = new ArrayList<Object>();
    ArrayList<Object> tdefs = new ArrayList<Object>();

    void $$IMPORTS()
    {
        Global.callFxn("loadPackage", xdcO, "xdc");
        Global.callFxn("loadPackage", xdcO, "xdc.corevers");
    }

    void $$OBJECTS()
    {
        pkgP = (Proto.Obj)om.bind("ti.sdo.ce.utils.rtcfg.Package", new Proto.Obj());
        pkgV = (Value.Obj)om.bind("ti.sdo.ce.utils.rtcfg", new Value.Obj("ti.sdo.ce.utils.rtcfg", pkgP));
    }

    void $$SINGLETONS()
    {
        pkgP.init("ti.sdo.ce.utils.rtcfg.Package", (Proto.Obj)om.findStrict("xdc.IPackage.Module", "ti.sdo.ce.utils.rtcfg"));
        Scriptable cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sdo/ce/utils/rtcfg/package.xs");
        om.bind("xdc.IPackage$$capsule", cap);
        Object fxn;
                fxn = Global.get(cap, "init");
                if (fxn != null) pkgP.addFxn("init", (Proto.Fxn)om.findStrict("xdc.IPackage$$init", "ti.sdo.ce.utils.rtcfg"), fxn);
                fxn = Global.get(cap, "close");
                if (fxn != null) pkgP.addFxn("close", (Proto.Fxn)om.findStrict("xdc.IPackage$$close", "ti.sdo.ce.utils.rtcfg"), fxn);
                fxn = Global.get(cap, "validate");
                if (fxn != null) pkgP.addFxn("validate", (Proto.Fxn)om.findStrict("xdc.IPackage$$validate", "ti.sdo.ce.utils.rtcfg"), fxn);
                fxn = Global.get(cap, "exit");
                if (fxn != null) pkgP.addFxn("exit", (Proto.Fxn)om.findStrict("xdc.IPackage$$exit", "ti.sdo.ce.utils.rtcfg"), fxn);
                fxn = Global.get(cap, "getLibs");
                if (fxn != null) pkgP.addFxn("getLibs", (Proto.Fxn)om.findStrict("xdc.IPackage$$getLibs", "ti.sdo.ce.utils.rtcfg"), fxn);
                fxn = Global.get(cap, "getSects");
                if (fxn != null) pkgP.addFxn("getSects", (Proto.Fxn)om.findStrict("xdc.IPackage$$getSects", "ti.sdo.ce.utils.rtcfg"), fxn);
        pkgP.bind("$capsule", cap);
        pkgV.init2(pkgP, "ti.sdo.ce.utils.rtcfg", Value.DEFAULT, false);
        pkgV.bind("$name", "ti.sdo.ce.utils.rtcfg");
        pkgV.bind("$category", "Package");
        pkgV.bind("$$qn", "ti.sdo.ce.utils.rtcfg.");
        pkgV.bind("$vers", Global.newArray("1, 0, 1"));
        Value.Map atmap = (Value.Map)pkgV.getv("$attr");
        atmap.seal("length");
        imports.clear();
        pkgV.bind("$imports", imports);
        StringBuilder sb = new StringBuilder();
        sb.append("var pkg = xdc.om['ti.sdo.ce.utils.rtcfg'];\n");
        sb.append("if (pkg.$vers.length >= 3) {\n");
            sb.append("pkg.$vers.push(Packages.xdc.services.global.Vers.getDate(xdc.csd() + '/..'));\n");
        sb.append("}\n");
        sb.append("pkg.build.libraries = [\n");
            sb.append("'lib/release/rtcfg_linux.av5T',\n");
            sb.append("'lib/release/rtcfg_dman3.av4TCE',\n");
            sb.append("'lib/release/rtcfg_dman3.a86U',\n");
            sb.append("'lib/release/rtcfg_dman3.a470uC',\n");
            sb.append("'lib/release/rtcfg_dman3.av5T',\n");
            sb.append("'lib/debug/rtcfg.av4TCE',\n");
            sb.append("'lib/debug/rtcfg.a86U',\n");
            sb.append("'lib/debug/rtcfg_dman3.av4TCE',\n");
            sb.append("'lib/debug/rtcfg.av5T',\n");
            sb.append("'lib/debug/rtcfg.a470uC',\n");
            sb.append("'lib/debug/rtcfg_dman3.a470uC',\n");
            sb.append("'lib/debug/rtcfg_linux.a86U',\n");
            sb.append("'lib/debug/rtcfg_linux.av5T',\n");
            sb.append("'lib/release/rtcfg.a86U',\n");
            sb.append("'lib/release/rtcfg.av5T',\n");
            sb.append("'lib/debug/rtcfg_dman3.a86U',\n");
            sb.append("'lib/release/rtcfg.av4TCE',\n");
            sb.append("'lib/debug/rtcfg_linux.a470uC',\n");
            sb.append("'lib/debug/rtcfg_dman3.av5T',\n");
            sb.append("'lib/release/rtcfg.a470uC',\n");
            sb.append("'lib/release/rtcfg_linux.a470uC',\n");
            sb.append("'lib/release/rtcfg_linux.a86U',\n");
        sb.append("];\n");
        sb.append("pkg.build.libDesc = [\n");
            sb.append("['lib/release/rtcfg_linux.av5T', {target: 'gnu.targets.arm.GCArmv5T'}],\n");
            sb.append("['lib/release/rtcfg_dman3.av4TCE', {target: 'microsoft.targets.arm.WinCE'}],\n");
            sb.append("['lib/release/rtcfg_dman3.a86U', {target: 'gnu.targets.Linux86'}],\n");
            sb.append("['lib/release/rtcfg_dman3.a470uC', {target: 'gnu.targets.UCArm9'}],\n");
            sb.append("['lib/release/rtcfg_dman3.av5T', {target: 'gnu.targets.arm.GCArmv5T'}],\n");
            sb.append("['lib/debug/rtcfg.av4TCE', {target: 'microsoft.targets.arm.WinCE'}],\n");
            sb.append("['lib/debug/rtcfg.a86U', {target: 'gnu.targets.Linux86'}],\n");
            sb.append("['lib/debug/rtcfg_dman3.av4TCE', {target: 'microsoft.targets.arm.WinCE'}],\n");
            sb.append("['lib/debug/rtcfg.av5T', {target: 'gnu.targets.arm.GCArmv5T'}],\n");
            sb.append("['lib/debug/rtcfg.a470uC', {target: 'gnu.targets.UCArm9'}],\n");
            sb.append("['lib/debug/rtcfg_dman3.a470uC', {target: 'gnu.targets.UCArm9'}],\n");
            sb.append("['lib/debug/rtcfg_linux.a86U', {target: 'gnu.targets.Linux86'}],\n");
            sb.append("['lib/debug/rtcfg_linux.av5T', {target: 'gnu.targets.arm.GCArmv5T'}],\n");
            sb.append("['lib/release/rtcfg.a86U', {target: 'gnu.targets.Linux86'}],\n");
            sb.append("['lib/release/rtcfg.av5T', {target: 'gnu.targets.arm.GCArmv5T'}],\n");
            sb.append("['lib/debug/rtcfg_dman3.a86U', {target: 'gnu.targets.Linux86'}],\n");
            sb.append("['lib/release/rtcfg.av4TCE', {target: 'microsoft.targets.arm.WinCE'}],\n");
            sb.append("['lib/debug/rtcfg_linux.a470uC', {target: 'gnu.targets.UCArm9'}],\n");
            sb.append("['lib/debug/rtcfg_dman3.av5T', {target: 'gnu.targets.arm.GCArmv5T'}],\n");
            sb.append("['lib/release/rtcfg.a470uC', {target: 'gnu.targets.UCArm9'}],\n");
            sb.append("['lib/release/rtcfg_linux.a470uC', {target: 'gnu.targets.UCArm9'}],\n");
            sb.append("['lib/release/rtcfg_linux.a86U', {target: 'gnu.targets.Linux86'}],\n");
        sb.append("];\n");
        sb.append("if('suffix' in xdc.om['xdc.IPackage$$LibDesc']) {\n");
            sb.append("pkg.build.libDesc['lib/release/rtcfg_linux.av5T'].suffix = 'v5T';\n");
            sb.append("pkg.build.libDesc['lib/release/rtcfg_dman3.av4TCE'].suffix = 'v4TCE';\n");
            sb.append("pkg.build.libDesc['lib/release/rtcfg_dman3.a86U'].suffix = '86U';\n");
            sb.append("pkg.build.libDesc['lib/release/rtcfg_dman3.a470uC'].suffix = '470uC';\n");
            sb.append("pkg.build.libDesc['lib/release/rtcfg_dman3.av5T'].suffix = 'v5T';\n");
            sb.append("pkg.build.libDesc['lib/debug/rtcfg.av4TCE'].suffix = 'v4TCE';\n");
            sb.append("pkg.build.libDesc['lib/debug/rtcfg.a86U'].suffix = '86U';\n");
            sb.append("pkg.build.libDesc['lib/debug/rtcfg_dman3.av4TCE'].suffix = 'v4TCE';\n");
            sb.append("pkg.build.libDesc['lib/debug/rtcfg.av5T'].suffix = 'v5T';\n");
            sb.append("pkg.build.libDesc['lib/debug/rtcfg.a470uC'].suffix = '470uC';\n");
            sb.append("pkg.build.libDesc['lib/debug/rtcfg_dman3.a470uC'].suffix = '470uC';\n");
            sb.append("pkg.build.libDesc['lib/debug/rtcfg_linux.a86U'].suffix = '86U';\n");
            sb.append("pkg.build.libDesc['lib/debug/rtcfg_linux.av5T'].suffix = 'v5T';\n");
            sb.append("pkg.build.libDesc['lib/release/rtcfg.a86U'].suffix = '86U';\n");
            sb.append("pkg.build.libDesc['lib/release/rtcfg.av5T'].suffix = 'v5T';\n");
            sb.append("pkg.build.libDesc['lib/debug/rtcfg_dman3.a86U'].suffix = '86U';\n");
            sb.append("pkg.build.libDesc['lib/release/rtcfg.av4TCE'].suffix = 'v4TCE';\n");
            sb.append("pkg.build.libDesc['lib/debug/rtcfg_linux.a470uC'].suffix = '470uC';\n");
            sb.append("pkg.build.libDesc['lib/debug/rtcfg_dman3.av5T'].suffix = 'v5T';\n");
            sb.append("pkg.build.libDesc['lib/release/rtcfg.a470uC'].suffix = '470uC';\n");
            sb.append("pkg.build.libDesc['lib/release/rtcfg_linux.a470uC'].suffix = '470uC';\n");
            sb.append("pkg.build.libDesc['lib/release/rtcfg_linux.a86U'].suffix = '86U';\n");
        sb.append("}\n");
        Global.eval(sb.toString());
    }

    void $$INITIALIZATION()
    {
        Value.Obj vo;

        if (isCFG) {
        }//isCFG
        Global.callFxn("init", pkgV);
        ((Value.Arr)om.findStrict("$packages", "ti.sdo.ce.utils.rtcfg")).add(pkgV);
    }

    public void exec( Scriptable xdcO, Session ses )
    {
        this.xdcO = xdcO;
        this.ses = ses;
        om = (Value.Obj)xdcO.get("om", null);

        Object o = om.geto("$name");
        String s = o instanceof String ? (String)o : null;
        isCFG = s != null && s.equals("cfg");
        isROV = s != null && s.equals("rov");

        $$IMPORTS();
        $$OBJECTS();
        if (isROV) {
        }//isROV
        $$SINGLETONS();
        $$INITIALIZATION();
    }
}