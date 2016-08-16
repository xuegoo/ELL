////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Project:  Embedded Machine Learning Library (EMLL)
//  File:     ValueSelectorNode.tcc (model)
//  Authors:  Chuck Jacobs
//
////////////////////////////////////////////////////////////////////////////////////////////////////

/// <summary> model namespace </summary>
namespace model
{
    template <typename ValueType>
    ValueSelectorNode<ValueType>::ValueSelectorNode() : Node({ &_condition, &_input1, &_input2 }, { &_output }), _condition(this, {}, conditionPortName), _input1(this, {}, input1PortName), _input2(this, {}, input2PortName), _output(this, outputPortName, 0)
    {
    }

    template <typename ValueType>
    ValueSelectorNode<ValueType>::ValueSelectorNode(const PortElements<bool>& condition, const PortElements<ValueType>& input1, const PortElements<ValueType>& input2) : Node({ &_condition, &_input1, &_input2 }, { &_output }), _condition(this, condition, conditionPortName), _input1(this, input1, input1PortName), _input2(this, input2, input2PortName), _output(this, outputPortName, input1.Size())
    {
        if (condition.Size() != 1)
        {
            throw std::runtime_error("Error: Condition must be 1-D signal");
        }

        if (input1.Size() != input2.Size())
        {
            throw std::runtime_error("Error: input values must be same dimension");
        }
    };

    template <typename ValueType>
    void ValueSelectorNode<ValueType>::Compute() const
    {
        bool cond = _condition[0];
        _output.SetOutput(cond ? _input1.GetValue() : _input2.GetValue());
    };

    template <typename ValueType>
    void ValueSelectorNode<ValueType>::Copy(ModelTransformer& transformer) const
    {
        auto newCondition = transformer.TransformPortElements(_condition.GetPortElements());
        auto newPortElements1 = transformer.TransformPortElements(_input1.GetPortElements());
        auto newPortElements2 = transformer.TransformPortElements(_input2.GetPortElements());

        auto newNode = transformer.AddNode<ValueSelectorNode<ValueType>>(newCondition, newPortElements1, newPortElements2);

         transformer.MapNodeOutput(output, newNode->output);
    }

    template <typename ValueType>
    void ValueSelectorNode<ValueType>::RefineNode(ModelTransformer& transformer) const
    {
        auto newCondition = transformer.TransformPortElements(_condition.GetPortElements());
        auto newPortElements1 = transformer.TransformPortElements(_input1.GetPortElements());
        auto newPortElements2 = transformer.TransformPortElements(_input2.GetPortElements());

        auto newNode = transformer.AddNode<ValueSelectorNode<ValueType>>(newCondition, newPortElements1, newPortElements2);

         transformer.MapNodeOutput(output, newNode->output);
    }

    //
    // Cheese
    //
    template <typename ValueType>
    
    SelectIfLessNode<ValueType>::SelectIfLessNode(const PortElements<ValueType>& input1, const PortElements<ValueType>& input2, const PortElements<ValueType>& value1, const PortElements<ValueType>& value2) : Node({ &_input1, &_input2, &_value1, &_value2 }, { &_output }), _input1(this, input1, input1PortName), _input2(this, input2, input2PortName), _value1(this, value1, value1PortName), _value2(this, value2, value2PortName), _output(this, outputPortName, input1.Size())
    {
        if (input1.Size() != input2.Size())
        {
            throw std::runtime_error("Error: input values must be same dimension");
        }

        if (value1.Size() != value2.Size())
        {
            throw std::runtime_error("Error: input values must be same dimension");
        }
    };

    template <typename ValueType>
    void SelectIfLessNode<ValueType>::Compute() const
    {
        auto in1 = _input1[0];
        auto in2 = _input2[0];
        bool cond = in1 < in2;
        _output.SetOutput(cond ? _value1.GetValue() : _value2.GetValue());
    };

    template <typename ValueType>
    void SelectIfLessNode<ValueType>::Copy(ModelTransformer& transformer) const
    {
        auto newPortElements1 = transformer.TransformPortElements(_input1.GetPortElements());
        auto newPortElements2 = transformer.TransformPortElements(_input2.GetPortElements());
        auto newValue1 = transformer.TransformPortElements(_value1.GetPortElements());
        auto newValue2 = transformer.TransformPortElements(_value2.GetPortElements());

        auto newNode = transformer.AddNode<SelectIfLessNode<ValueType>>(newPortElements1, newPortElements2, newValue1, newValue2);
        transformer.MapNodeOutput(_output, newNode->output);
    }

    template <typename ValueType>
    void SelectIfLessNode<ValueType>::RefineNode(ModelTransformer& transformer) const
    {
        auto newPortElements1 = transformer.TransformPortElements(_input1.GetPortElements());
        auto newPortElements2 = transformer.TransformPortElements(_input2.GetPortElements());
        auto newValue1 = transformer.TransformPortElements(_value1.GetPortElements());
        auto newValue2 = transformer.TransformPortElements(_value2.GetPortElements());

        auto ifLessNode = transformer.AddNode<IfLessNode<ValueType>>(newPortElements1, newPortElements2);
        auto selectNode = transformer.AddNode<ValueSelectorNode<ValueType>>(ifLessNode->output, newValue1, newValue2);

        transformer.MapNodeOutput(_output, selectNode->output);
    }

    //
    // More cheese
    //
    template <typename ValueType>
    IfLessNode<ValueType>::IfLessNode(const PortElements<ValueType>& value, const PortElements<ValueType>& threshold) : Node({ &_value, &_threshold }, { &_output }), _value(this, value, valuePortName), _threshold(this, threshold, thresholdPortName), _output(this, outputPortName, 1)
    {
        if (value.Size() != threshold.Size())
        {
            throw std::runtime_error("Error: input values must be same dimension");
        }
    };

    template <typename ValueType>
    void IfLessNode<ValueType>::Compute() const
    {
        std::vector<bool> output;
        output.push_back(_value[0] < _threshold[0]);
        _output.SetOutput(output);
    };

    template <typename ValueType>
    void IfLessNode<ValueType>::Copy(ModelTransformer& transformer) const
    {
        auto newValue = transformer.TransformPortElements(_value.GetPortElements());
        auto newThreshold = transformer.TransformPortElements(_threshold.GetPortElements());

        auto newNode = transformer.AddNode<IfLessNode<ValueType>>(newValue, newThreshold);

        transformer.MapNodeOutput(output, newNode->output);
    }

    template <typename ValueType>
    void IfLessNode<ValueType>::RefineNode(ModelTransformer& transformer) const
    {
        auto newValue = transformer.TransformPortElements(_value.GetPortElements());
        auto newThreshold = transformer.TransformPortElements(_threshold.GetPortElements());

        auto newNode = transformer.AddNode<IfLessNode<ValueType>>(newValue, newThreshold);

         transformer.MapNodeOutput(output, newNode->output);
    }
}
