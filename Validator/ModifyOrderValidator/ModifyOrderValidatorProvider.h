class ModifyOrderValidatorFactory final: public IValidatorFactory
{
    std::unique_ptr<IRequestValidator> CreateValidator() const override 
    {
        return std::make_unique<ModifyOrderValidator>();
    }
};