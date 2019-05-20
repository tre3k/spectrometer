/* Init.java (c) Kirill Pshenichnyi 2019

 */

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;

public class Init extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        ExecInit(req,resp);
    }

    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        ExecInit(req,resp);
    }

    public void ExecInit(HttpServletRequest req, HttpServletResponse resp) throws ServletException,IOException{
        Machine machine = new Machine();
        machine.Test();
        machine.Init();
        machine.close();

        ReadMem.count = 0;

        resp.getWriter().println("init");
    }
}
