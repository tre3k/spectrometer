import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;

public class Start extends HttpServlet {


    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        ExecStart(req,resp);
    }

    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        ExecStart(req,resp);
    }

    public void ExecStart(HttpServletRequest req, HttpServletResponse resp) throws ServletException,IOException{
        /* set parameters for number of channels (in code) and width of channels */
        int channels = Integer.valueOf(req.getParameter("channels"));
        int width = Integer.valueOf(req.getParameter("channels"));

        

    }
}